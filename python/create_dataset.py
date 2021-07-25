import numpy as np
import os

from tensorflow.keras.utils import to_categorical

DATASET_PATH = "./dataset"

DATA_SHAPE = (80000, 2)

LABEL = "left", "front", "right"

features = np.zeros(shape=(0, *DATA_SHAPE), dtype=np.int16)
labels = np.zeros(shape=(0, len(LABEL)), dtype=np.float32)

for index, label in enumerate(LABEL):
    for data in [ np.load(os.path.join(DATASET_PATH, label, _)) for _ in os.listdir(os.path.join(DATASET_PATH, label)) if _.endswith(".npy")]:
        features = np.append(features, [data], axis=0)

        labels = np.append(labels, to_categorical([index], len(LABEL)), axis=0)

r = np.random.permutation(len(labels))

features = features[r]
labels = labels[r]

np.savez("dataset.npz", features=features, labels=labels)