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

class GraphCodeDataset(TorchDataset):
    def __init__(self, encodings, labels):
        self.encodings = encodings
        self.labels = labels

    def __getitem__(self, idx):
        item = {key: val[idx].clone().detach() for key, val in self.encodings.items()}
        item['labels'] = torch.tensor(self.labels[idx])
        return item

    def __len__(self):
        return len(self.labels)

class GraphCodeBERTTrainer:
    def __init__(self, model_dir=None):
        self.model_dir = model_dir if model_dir else "./models/graphcodebert_model"
        os.makedirs(self.model_dir, exist_ok=True)
        
        self.tokenizer = AutoTokenizer.from_pretrained("microsoft/graphcodebert-base")
        
        model_path = self.model_dir if os.path.exists(os.path.join(self.model_dir, "config.json")) else "microsoft/graphcodebert-base"
        self.model = RobertaForSequenceClassification.from_pretrained(
            model_path,
            num_labels=2
        ).to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

    def _preprocess_code(self, code):
        code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
        code = re.sub(r'//.*$', '', code, flags=re.MULTILINE)
        return code.strip()

    def create_dataset(self, code_files, labels):
        codes = [self._preprocess_code(Path(f).read_text()) for f in code_files]
        encodings = self.tokenizer(
            codes,
            padding="max_length",
            truncation=True,
            max_length=512,
            return_tensors="pt"
        )
        return GraphCodeDataset(encodings, labels)

    def train(self, train_files, train_labels, epochs=3):
        try:
            train_dataset = self.create_dataset(train_files, train_labels)
            
            training_args = TrainingArguments(
                output_dir=self.model_dir,
                per_device_train_batch_size=8,
                num_train_epochs=epochs,
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
            print(f"Trained on {len(train_files)} samples. Model saved to {self.model_dir}")
            return True
            
        except Exception as e:
            print(f"Training failed: {str(e)}")
            return False

    def predict(self, code_file):
        self.model.eval()
        try:
            code = self._preprocess_code(Path(code_file).read_text())
            inputs = self.tokenizer(
                code,
                return_tensors="pt",
                truncation=True,
                max_length=512,
                padding="max_length"
            ).to(self.model.device)
            
            with torch.no_grad():
                outputs = self.model(**inputs)
            
            probs = torch.softmax(outputs.logits, dim=1)[0]
            pred = torch.argmax(probs).item()
            confidence = probs[pred].item() * 100
            
            return pred, confidence
            
        except Exception as e:
            print(f"Prediction error: {str(e)}")
            return -1, 0.0

if __name__ == "__main__":
    import argparse
    import glob

    parser = argparse.ArgumentParser(description="GraphCodeBERT Vulnerability Detector")
    subparsers = parser.add_subparsers(dest='command', required=True)
    
    # Single-file training
    train_parser = subparsers.add_parser('train')
    train_parser.add_argument('code_files', nargs='+', help="Code file(s) to train on")
    train_parser.add_argument('--label', type=int, required=True, choices=[0,1], 
                            help="0 for safe, 1 for vulnerable")
    train_parser.add_argument('--epochs', type=int, default=3)
    train_parser.add_argument('--model_dir', default="./models/graphcodebert_model")

    # Directory-based training (original functionality)
    dir_train_parser = subparsers.add_parser('dir-train')
    dir_train_parser.add_argument('--train_dir', required=True, 
                                help="Directory with vulnerable/ and safe/ subdirs")
    dir_train_parser.add_argument('--epochs', type=int, default=3)
    dir_train_parser.add_argument('--model_dir', default="./models/graphcodebert_model")

    # Prediction
    predict_parser = subparsers.add_parser('predict')
    predict_parser.add_argument('code_file', help="Code file to analyze")
    predict_parser.add_argument('--model_dir', default="./models/graphcodebert_model")

    args = parser.parse_args()

    if args.command == 'train':
        trainer = GraphCodeBERTTrainer(args.model_dir)
        trainer.train(
            args.code_files,
            [args.label] * len(args.code_files),
            args.epochs
        )

    elif args.command == 'dir-train':
        # Original directory-based training
        vuln_files = glob.glob(os.path.join(args.train_dir, "vulnerable", "*.c"))
        safe_files = glob.glob(os.path.join(args.train_dir, "safe", "*.c"))
        trainer = GraphCodeBERTTrainer(args.model_dir)
        trainer.train(
            vuln_files + safe_files,
            [1]*len(vuln_files) + [0]*len(safe_files),
            args.epochs
        )

    elif args.command == 'predict':
        trainer = GraphCodeBERTTrainer(args.model_dir)
        pred, confidence = trainer.predict(args.code_file)
        
        if pred == 0:
            print(f"SAFE (0) | Confidence: {confidence:.2f}%")
        elif pred == 1:
            print(f"VULNERABLE (1) | Confidence: {confidence:.2f}%")
        else:
            print("ERROR | Could not process the file")
