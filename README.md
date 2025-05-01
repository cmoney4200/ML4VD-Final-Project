We tested 4 pre-trained ML4VD (Machine Learning for Vulnerability Defense) models against 10 different semantically equivalent data transformations.
While the models did come pretrained on other datasets, they were not previously trained on the datasets used in our experiment.
The models are trained on individual vulnerable files or in a pair of one vulnerable file and one good file.
All datasets were trained on code within the same folder, so some adjustments to file paths may need to be changed for future training and testing.
Each of the four models comes with its own training file, all ran in a similar way.
To train one of the models on a single vulnerable file an example is:
python codebert_trainer.py train bad_code.c --label 1
1 denotes that the file is vulnerable to the model, similarly to train the model on a single safe file you run:
python codebert_trainer.py train good_code.c --label 0
0 denotes a safe file in this case.
To train on a piece of vulnerable and a piece of safe code at the same time you would run:
python codebert_trainer.py train bad_code.c --safe_example good_code.c
For our experiment all four models were only trained on the vulnerable and safe code with no transformations applied.
Models were not trained on the transformed versions of vulnerable or safe code.
Each model trainer file also makes predictions about whether files are vulnerable by running the command:
python codebert_trainer.py predict bad_code.c
