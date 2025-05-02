#!/usr/bin/env python3
import torch
import re
from pathlib import Path
from transformers import (
    RobertaForSequenceClassification,
    AutoTokenizer,
    Trainer,
    TrainingArguments
)
import numpy as np
import os
from torch.utils.data import Dataset as TorchDataset

class UniXcoderDataset(TorchDataset):
    def __init__(self, encodings, labels):
        self.encodings = encodings
        self.labels = labels

    def __getitem__(self, idx):
        item = {key: val[idx].clone().detach() for key, val in self.encodings.items()}
        item['labels'] = torch.tensor(self.labels[idx])
        return item

    def __len__(self):
        return len(self.labels)

class UniXcoderTrainer:
    def __init__(self, model_dir=None):
        self.model_dir = model_dir if model_dir else "./models/unixcoder_model"
        os.makedirs(self.model_dir, exist_ok=True)
        
        #Calling UnixCoder tokenizer
        self.tokenizer = AutoTokenizer.from_pretrained("microsoft/unixcoder-base")
        
        model_path = self.model_dir if os.path.exists(os.path.join(self.model_dir, "config.json")) else "microsoft/unixcoder-base"
        self.model = RobertaForSequenceClassification.from_pretrained(
            model_path,
            num_labels=2  # 0=Safe, 1=Vulnerable
        ).to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

    def _preprocess_code(self, code):
        #Not considered in initial setup in regards to transformation 9
        code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)  # Remove block comments
        code = re.sub(r'//.*$', '', code, flags=re.MULTILINE)   # Remove line comments
        return code.strip()

    def create_dataset(self, code_files, labels):
        codes = [self._preprocess_code(Path(f).read_text()) for f in code_files]
        
        #UniXcoder tokenization
        encodings = self.tokenizer(
            codes,
            padding="max_length",
            truncation=True,
            max_length=512,
            return_tensors="pt",
            return_token_type_ids=True 
        )
        return UniXcoderDataset(encodings, labels)

    def train(self, code_files, labels, epochs=1):
        try:
            train_dataset = self.create_dataset(code_files, labels)
            
            #Training arguments optimized for UniXcoder
            training_args = TrainingArguments(
                output_dir=self.model_dir,
                per_device_train_batch_size=1,
                num_train_epochs=epochs,
                learning_rate=2e-5,  #Gradual learning rate for our one file at a time training method, may have increased prediction accuracy
                weight_decay=0.01,
                save_strategy="no",
                logging_strategy="no",
                report_to="none",
                disable_tqdm=True
            )
            
            trainer = Trainer(
                model=self.model,
                args=training_args,
                train_dataset=train_dataset,
            )
            
            trainer.train()
            self.model.save_pretrained(self.model_dir)
            self.tokenizer.save_pretrained(self.model_dir)
            print(f"Trained on {len(code_files)} samples. Model saved to {self.model_dir}")
            return True
            
        except Exception as e:
            print(f"UniXcoder training failed: {str(e)}")
            return False

    def predict(self, code_file):
        self.model.eval()
        try:
            code = self._preprocess_code(Path(code_file).read_text())
            
            #UniXcoder requires token_type_ids
            inputs = self.tokenizer(
                code,
                return_tensors="pt",
                truncation=True,
                max_length=512,
                padding="max_length",
                return_token_type_ids=True
            ).to(self.model.device)
            
            with torch.no_grad():
                outputs = self.model(**inputs)
            
            probs = torch.softmax(outputs.logits, dim=1)[0]
            pred = torch.argmax(probs).item()
            confidence = probs[pred].item() * 100
            
            return pred, confidence
            
        except Exception as e:
            print(f"UniXcoder prediction error: {str(e)}")
            return -1, 0.0

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="UniXcoder Vulnerability Detector")
    subparsers = parser.add_subparsers(dest='command', required=True)
    
    #Training arguments
    train_parser = subparsers.add_parser('train')
    train_parser.add_argument('code_files', nargs='+', help="Code file(s) to train on")
    train_parser.add_argument('--label', type=int, required=True, choices=[0,1], 
                            help="0 for safe, 1 for vulnerable")
    train_parser.add_argument('--epochs', type=int, default=1)
    train_parser.add_argument('--model_dir', default="./models/unixcoder_model")

    #Prediction arguments
    predict_parser = subparsers.add_parser('predict')
    predict_parser.add_argument('code_file', help="Code file to analyze")
    predict_parser.add_argument('--model_dir', default="./models/unixcoder_model")

    args = parser.parse_args()

    if args.command == 'train':
        trainer = UniXcoderTrainer(args.model_dir)
        trainer.train(
            args.code_files,
            [args.label] * len(args.code_files),
            args.epochs
        )

    elif args.command == 'predict':
        trainer = UniXcoderTrainer(args.model_dir)
        pred, confidence = trainer.predict(args.code_file)
        
        if pred == 0:
            print(f"SAFE (0) | Confidence: {confidence:.2f}%")
        elif pred == 1:
            print(f"VULNERABLE (1) | Confidence: {confidence:.2f}%")
        else:
            print("ERROR | Could not process the file")
