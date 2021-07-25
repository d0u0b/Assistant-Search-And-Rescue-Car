import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.layers import Bidirectional, LSTM, Dropout, Dense, Flatten, Input

input_shape = (80000, 2)
from tensorflow.python.client import device_lib

print(device_lib.list_local_devices())
exit()

# tf.
model = Sequential()
model.add(Input(shape = input_shape))
model.add(Bidirectional(LSTM(128, return_sequences = True)))
model.add(Bidirectional(LSTM(64, return_sequences = True)))
model.add(Dropout(0.2))
model.add(Bidirectional(LSTM(32, return_sequences = True)))
model.add(Bidirectional(LSTM(16, return_sequences = False)))
model.add(Dropout(0.2))
model.add(Dense(3, activation="softmax"))


model.compile(
    loss = 'categorical_crossentropy', 
    optimizer = Adam(lr = 0.001), 
    metrics = ['acc']
)


model.summary()