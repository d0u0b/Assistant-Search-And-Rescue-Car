import numpy as np
from tqdm import tqdm

dataset = np.load('./dataset.npz')
features = dataset["features"]
labels = dataset["labels"]

train_features = np.zeros((0, 80000, 2), dtype=np.int16)
train_labels = np.zeros((0, 3), dtype=np.float)

for i in tqdm(range(50), total=50):
    train_features = np.append(train_features, [features[i]], axis=0)
    train_labels = np.append(train_labels, [labels[i]], axis=0)

    for i in range(10):
        train_features = np.append(train_features, [features[i] + np.random.randint(-10, 10, (80000, 2))], axis=0)
        train_labels = np.append(train_labels, [labels[i]], axis=0)

val_features = np.zeros((0, 80000, 2), dtype=np.int16)
val_labels = np.zeros((0, 3), dtype=np.float)

for i in tqdm(range(14), total=14):
    val_features = np.append(val_features, [features[50 + i]], axis=0)
    val_labels = np.append(val_labels, [labels[50 + i]], axis=0)

    for i in range(10):
        val_features = np.append(val_features, [features[50 + i] + np.random.randint(-10, 10, (80000, 2))], axis=0)
        val_labels = np.append(val_labels, [labels[50 + i]], axis=0)

r = np.random.permutation(len(train_labels))

train_features = train_features[r]
train_labels = train_labels[r]

r = np.random.permutation(len(val_labels))

val_features = val_features[r]
val_labels = val_labels[r]

np.savez("dataset_inc.npz",
    train_features = train_features,
    train_labels = train_labels,
    val_features = val_features,
    val_labels = val_labels
)