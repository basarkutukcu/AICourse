import tensorflow as tf
from tensorflow.keras.layers import Dense, Conv2D
from tensorflow.keras import constraints
from tensorflow.keras import initializers
from tensorflow.keras import backend as K
from tensorflow.python.framework import tensor_shape
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import MaxPooling2D, Flatten, Activation, BatchNormalization
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.initializers import GlorotNormal

def clip(x):
    return tf.math.maximum(-1.,tf.math.minimum(1.,x))

def quantize(ri, k):
    level = tf.math.pow(2., k) - 1.
    ro = (1. / level) * tf.math.round(level * ri)
    return ri + tf.stop_gradient(ro - ri)


class EnvDense(Dense):

    def __init__(self, units, kSet, **kwargs):
        super(EnvDense, self).__init__(units, **kwargs)
        self.k = kSet

    def build(self, input_shape):
        input_shape = tensor_shape.TensorShape(input_shape)
        last_dim = tensor_shape.dimension_value(input_shape[-1])
        # print(last_dim)

        self.kernel = self.add_weight(shape=(last_dim, self.units),
                                     initializer=self.kernel_initializer,
                                     name='kernel',
                                     regularizer=self.kernel_regularizer,
                                     constraint=self.kernel_constraint)

        if self.use_bias:
            self.bias = self.add_weight(shape=[self.units],
                                     initializer=self.bias_initializer,
                                     name='bias',
                                     regularizer=self.bias_regularizer,
                                     constraint=self.bias_constraint)
        else:
            self.bias = None

        self.built = True

    def call(self, inputs):
        wb = self.quantOpt(self.kernel)
        output = K.dot(inputs, wb)
        if self.use_bias:
            output = K.bias_add(output, self.bias)
        return output

    def quantOpt(self, x):
        xc = clip(x)
        xq = quantize(xc, k=self.k)
        return xq

    def setK(self, kSet):
        self.k = kSet

    def getK(self):
        return self.k


class EnvConv2D(Conv2D):

    def __init__(self, filters, kSet, **kwargs):
        super(EnvConv2D, self).__init__(filters, **kwargs)
        self.k = kSet

    def build(self, input_shape):
        input_shape = tensor_shape.TensorShape(input_shape)
        input_channel = self._get_input_channel(input_shape)
        kernel_shape = self.kernel_size + (input_channel, self.filters)

        self.kernel = self.add_weight(shape=kernel_shape,
                                 initializer=self.kernel_initializer,
                                 name='kernel',
                                 regularizer=self.kernel_regularizer,
                                 constraint=self.kernel_constraint)
        if self.use_bias:
            self.bias = self.add_weight(
									 shape=(self.filters,),
                                     initializer=self.bias_initializer,
                                     name='bias',
                                     regularizer=self.bias_regularizer,
                                     constraint=self.bias_constraint)
        else:
            self.bias = None
        self.built = True

    def call(self, inputs):
        wb = self.quantOpt(self.kernel) 
        outputs = K.conv2d(
            inputs,
            wb,
            strides=self.strides,
            padding=self.padding,
            data_format=self.data_format,
            dilation_rate=self.dilation_rate)

        if self.use_bias:
            outputs = K.bias_add(
                outputs,
                self.bias,
                data_format=self.data_format)

        return outputs

    def quantOpt(self, x):
        xc = clip(x)
        xq = quantize(xc, k=self.k)
        return xq

    def setK(self, kSet):
        self.k = kSet

    def getK(self):
        return self.k


def EnvModel(inpShape):
    # define model
    model = Sequential()

    model.add(EnvConv2D(32,8, kernel_size=(5,5), kernel_initializer=GlorotNormal(),
                    input_shape=inpShape))
    model.add(BatchNormalization(epsilon=1e-5, momentum=0.99))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(EnvConv2D(64,8, kernel_size=(5,5), kernel_initializer=GlorotNormal()))
    model.add(BatchNormalization(epsilon=1e-5, momentum=0.99))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Flatten())
    model.add(EnvDense(512,8, kernel_initializer=GlorotNormal()))
    model.add(BatchNormalization(epsilon=1e-5, momentum=0.99))
    model.add(Activation('relu'))

    model.add(EnvDense(10,8, kernel_initializer=GlorotNormal()))
    model.add(BatchNormalization(epsilon=1e-5, momentum=0.99))
    model.add(Activation('softmax'))

    model.compile(loss='categorical_crossentropy', optimizer=Adam(lr=1e-4), metrics=['accuracy'])
    model.load_weights('mnistBase.h5')

    return model