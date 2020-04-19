import tensorflow as tf
import numpy as np
from tensorflow.keras.optimizers import Adam

inds = [0,4,9,12]

def getStates(model, curInd, inds):
    states = []
    states.append(curInd)
    for i in inds:
        l = model.get_layer(index = i)
        states.append(l.getK())
    return tuple(states)

def getReward(model, xTest, yTest):
    accOriginal = 99.50
    loss,accQuant = model.evaluate(xTest, yTest,verbose=0)
    return 0.1*(accQuant*100 - accOriginal)

def applyAction(model, a, layerIndex):
    lInd = inds[layerIndex]
    l = model.get_layer(index=lInd)
    l.setK(a)
    model.compile(loss='categorical_crossentropy', optimizer=Adam(lr=1e-4), metrics=['accuracy'])

def reTrain(model, ep, xTrain, yTrain, xTest, yTest):
    hist = model.fit(xTrain, yTrain, epochs=ep, batch_size=128,
                    validation_data=(xTest,yTest),verbose=0)
    return hist.history['val_accuracy'][0]


