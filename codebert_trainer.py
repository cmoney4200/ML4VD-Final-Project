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
import json
from torch.utils.data import Dataset as TorchDataset

class CodeDataset(TorchDataset):
    def __init__(self, encodings, labels):
        self.encodings = encodings
        self.labels = labels

    def __getitem__(self, idx):
        #Retrieving tokenized data from index idx
        item = {key: val[idx].clone().detach() for key, val in self.encodings.items()}
        #Creates a dictionary w/ cloned and detached sensors to avoid modifying the og data
        item['labels'] = torch.tensor(self.labels[idx])
        return item

    def __len__(self):
        #Returns # of samples in dataset, in our case was always 1
        return len(self.labels)

class IncrementalCodeBERTTrainer:
    def __init__(self, model_dir=None):
        #Looks for model directory and creates one if not present
        self.model_dir = model_dir if model_dir else "./models/codebert_model"
        os.makedirs(self.model_dir, exist_ok=True)
        #Loads pre-trained CodeBERT from hugging face
        self.tokenizer = AutoTokenizer.from_pretrained("microsoft/codebert-base")
        
        #Looking for pre-trained model
        model_path = self.model_dir if os.path.exists(os.path.join(self.model_dir, "config.json")) else "microsoft/codebert-base"
        #num_labels set to 2 for binary classification (vulnerable or safe)
        self.model = RobertaForSequenceClassification.from_pretrained(
            model_path,
            num_labels=2
        ).to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

    def _preprocess_code(self, code):
        #Removing multiline, single line comments, and excess whitespace from the code
        #Note: was not considered in regards to transformation 9, removing comments
        code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
        code = re.sub(r'//.*$', '', code, flags=re.MULTILINE)
        return code.strip()

    def create_single_example(self, code_file, label, safe_example=None):
        #Training based on one file at a time and an optional safe file for training comparison
        # Read and preprocess vulnerable code
        vuln_code = self._preprocess_code(Path(code_file).read_text())
        
        #Initialzing lists w/ vulnerable code and label (1)
        codes = [vuln_code]
        labels = [label]
        
        
        #Tokenize w/ CodeBERT tokenizer
        encodings = self.tokenizer(
            codes,
            padding="max_length",
            truncation=True,
            max_length=512,
            return_tensors="pt"
        )
        
        return encodings, labels

    def train_on_single_example(self, code_file, label=1, safe_example=None):
        #Training one vulnerable example at a time
        try:
            #Create training data using prevous tokenizer function and wrap in CodeDataset object
            encodings, labels = self.create_single_example(code_file, label)
            train_dataset = CodeDataset(encodings, labels)
            
            #Setting training argument parameters
            training_args = TrainingArguments(
                output_dir=self.model_dir,
                per_device_train_batch_size=1,
                num_train_epochs=1,
                save_strategy="no",
                logging_strategy="no",
                report_to="none",
                disable_tqdm=True
            )
            
            #Initialize hugging face "trainer" w/ model, training arguments, and the dataset
            trainer = Trainer(
                model=self.model,
                args=training_args,
                train_dataset=train_dataset,
            )
            
            #Run trainer on individual vulnerable code or vuln safe code pair
            trainer.train()
            
            #Save newly trained model and tokenizer
            self.model.save_pretrained(self.model_dir)
            self.tokenizer.save_pretrained(self.model_dir)
            
            print(f"Successfully trained on {code_file}")
            return True
            
        except Exception as e:
            print(f"Training failed on {code_file}: {str(e)}")
            return False

    def predict(self, code_file):
        #Predicting whether an individual file is vulnerable or not
        #Sets model to evaluation mode
        self.model.eval() 
        
        try:
            #Read and tokenize given code
            code = Path(code_file).read_text()
            code = self._preprocess_code(code)
            
            #Tokenize and move code to model
            inputs = self.tokenizer(
                code,
                return_tensors="pt",
                truncation=True,
                max_length=512,
                padding="max_length"
            ).to(self.model.device)
            
            #Not using gradients, using inference mode, since model weights are not being adjusted
            with torch.no_grad():
                outputs = self.model(**inputs)
            
            #Applying softmax to logits to get the probabilities
            probs = torch.softmax(outputs.logits, dim=1)[0]
            #Gets binary output of 0 or 1, safe or vulnerable, using argmax
            pred = torch.argmax(probs).item()
            #Calculating confidence as a percentage
            confidence = probs[pred].item() * 100
            
            return pred, confidence
            
        except Exception as e:
            print(f"Prediction error on {code_file}: {str(e)}")
            return -1, 0.0

if __name__ == "__main__":
    #For command line arguments
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Incremental CodeBERT Training for Vulnerability Detection",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    subparsers = parser.add_subparsers(dest='command', required=True)
    
    #Training set up
    train_parser = subparsers.add_parser('train', help='Train on a single code example')
    train_parser.add_argument('code_file', help="C code file to train on")
    train_parser.add_argument('--label', type=int, choices=[0,1], default=1,
                            help="0 for safe, 1 for vulnerable")
    train_parser.add_argument('--model_dir', default="./models/codebert_model",
                            help="Directory to save/load model")
    
    #Predict setup
    predict_parser = subparsers.add_parser('predict', help='Predict vulnerability')
    predict_parser.add_argument('code_file', help="Code file to analyze")
    predict_parser.add_argument('--model_dir', default="./models/codebert_model",
                              help="Directory containing trained model")
    
    args = parser.parse_args()
    
    if args.command == 'train':
        trainer = IncrementalCodeBERTTrainer(args.model_dir)
        success = trainer.train_on_single_example(
            args.code_file,
            args.label,
            args.safe_example
        )
        if success:
            print(f"Model updated and saved to {args.model_dir}")
        else:
            print("Training failed")
    
    elif args.command == 'predict':
        trainer = IncrementalCodeBERTTrainer(args.model_dir)
        prediction, confidence = trainer.predict(args.code_file)
        
        if prediction == 0:
            print(f"SAFE (0) | Confidence: {confidence:.2f}%")
        elif prediction == 1:
            print(f"VULNERABLE (1) | Confidence: {confidence:.2f}%")
        else:
            print("ERROR | Could not process the file")
