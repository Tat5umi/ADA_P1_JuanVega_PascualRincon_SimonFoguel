import pandas as pd

file = pd.read_csv("experiment_4.csv")
max = 0
for i in file['ocupacion_buffer']:
    if i > max:
        max = i

print(max)
