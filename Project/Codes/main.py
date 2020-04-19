import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from tensorflow.keras.datasets import mnist
from tensorflow.keras.constraints import min_max_norm
from tensorflow.keras import backend as K
from tensorflow.keras.initializers import GlorotNormal
from envModel import EnvDense, EnvConv2D
from tensorflow.keras.models import load_model

from envModel import EnvModel

import keras.backend.tensorflow_backend as tfback
print("tf.__version__ is", tf.__version__)
print("tf.keras.__version__ is:", tf.keras.__version__)

def _get_available_gpus():
    """Get a list of available gpu devices (formatted as strings).

    # Returns
        A list of available GPU devices.
    """
    #global _LOCAL_DEVICES
    if tfback._LOCAL_DEVICES is None:
        devices = tf.config.list_logical_devices()
        tfback._LOCAL_DEVICES = [x.name for x in devices]
    return [x for x in tfback._LOCAL_DEVICES if 'device:gpu' in x.lower()]

tfback._get_available_gpus = _get_available_gpus

#Load the data
(X_train, y_train), (X_test, y_test) = mnist.load_data()
X_train = X_train.reshape(60000, 28, 28, 1)
X_test = X_test.reshape(10000, 28, 28, 1)
X_train = X_train.astype('float32')
X_test = X_test.astype('float32')
X_train /= 255
X_test /= 255

#one-hot encoding
y_train = tf.keras.utils.to_categorical(y_train, 10) # * 2 - 1
y_test = tf.keras.utils.to_categorical(y_test, 10) # * 2 - 1

print(X_train.shape, 'train samples')
print(X_test.shape, 'test samples')

model = EnvModel()

#construct state space
states = []
for currLayer in range(4):
    for l0 in range(2,8):
        for l1 in range(2,8):
            for l2 in range(2,8):
                for l3 in range(2,8):
                    states.append((currLayer,l0,l1,l2,l3))

Q = {}
for s in states:
    for a in range(2,8):
        Q[s, a] = 0




