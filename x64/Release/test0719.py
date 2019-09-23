import numpy as np
np.set_printoptions(threshold=np.inf)
from keras.models import Sequential
from keras.optimizers import Adam
from keras.preprocessing.image import ImageDataGenerator
from keras.models import load_model
import pandas as pd



def classifier_pridictor_keras(photo_path):
    test_path = photo_path
    # print(test_path)
    test_datagen = ImageDataGenerator(rescale=1. / 255)
    test_batches = test_datagen.flow_from_directory(test_path, target_size=(50, 50),
                                                    classes=['doubles', 'microdoubs', 'nons', 'singles', 'triples'],
                                                    batch_size=64, save_format='jpeg', shuffle=False)

    model = Sequential()
    model = load_model('E:/tmp/fullmodel0714$100.h5')
    # model.summary()

    model.compile(optimizer=Adam(lr=0.001, beta_1=0.9, beta_2=0.999, epsilon=None, decay=1e-6, amsgrad=False),
                  loss='categorical_crossentropy', metrics=['accuracy'], loss_weights=None, sample_weight_mode=None,
                  weighted_metrics=None, target_tensors=None)


    predictions = model.predict_generator(test_batches, steps=len(test_batches), max_queue_size=10, workers=1,
                                          use_multiprocessing=False, verbose=0)
    # print(test_batches.filenames)
    pd.set_option('max_rows', 800)
    m1 = np.argmax(np.round(predictions), axis=1)
    # print(m1)

    str = ','.join(test_batches.filenames)
    # print(str)

    str2 = ",".join('%s' % id for id in m1)
    # print(str2)



    full_name = str + ',' + str2
    # print(full_name)


    # m3 = ['singles\\15.jpeg', 'singles\\19 (2).jpeg', 'singles\\19.jpeg', 'singles\\3.jpeg', 'singles\\86.jpeg'];
    # print(m3)
    #
    # m2 = [3, 4, 2, 0, 0]
    #
    # str = ','.join(m3)
    # print(str)

    return full_name

