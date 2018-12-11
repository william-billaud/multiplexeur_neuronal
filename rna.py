import numpy as np
import pandas as pd  # For loading and processing the dataset
import tensorflow as tf  # Of course, we need TensorFlow.
from sklearn.model_selection import train_test_split

# Read the CSV input
train = pd.read_csv('./data.csv', header=None, usecols=[i for i in range(1, 21)], delimiter=";")

# normalization des données
mean = []
std = []
for i in range(1, 20):
    mean.insert(i, train[i].mean())
    std.insert(i, train[i].std())

# pour pouvoir soustraire/diviser les series, il faut que elles aient la même taille
mean.insert(20, 0)
std.insert(20, 1)
train = (train - mean) / std

# genère la serie de resultat attendu
for i in range(1, 5):
    train[20 + i] = (train[20] == i).astype(int)

# generation du jeux de test :
x_train = train.drop(20, axis=1).values
y_train = train[20].values

entree = tf.placeholder(tf.float32, name='entree', shape=(None, x_train.shape[1]))
label = tf.placeholder(tf.float32, shape=(None, 4), name='labels')

labels_train = (np.arange(4) == y_train[:, None]).astype(np.float32)
hid1_size = 256
hid2_size = 128

w1 = tf.Variable(tf.random.normal(shape=[hid1_size, x_train.shape[1]], stddev=0.01), name='w1')
b1 = tf.Variable(tf.constant(0.1, shape=[hid1_size, 1]), name='b1')
y1 = tf.nn.sigmoid(tf.add(tf.matmul(w1, tf.transpose(entree)), b1))

w2 = tf.Variable(tf.random.normal(shape=[hid2_size, hid1_size], stddev=0.01), name='w2')
b2 = tf.Variable(tf.constant(0.1, shape=[hid2_size, 1]), name='b2')
y2 = tf.nn.sigmoid(tf.add(tf.matmul(w2, y1), b2))
print("here \n")
wo = tf.Variable(tf.random.normal([4, hid2_size], stddev=0.01), name='wo')
bo = tf.Variable(tf.random_normal([4, 1]), name='bo')
yo = tf.transpose(tf.add(tf.matmul(wo, y2), bo))

lr = tf.placeholder(tf.float32, shape=(), name='learning_rate')
loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=yo, labels=label))
optimizer = tf.train.GradientDescentOptimizer(lr).minimize(loss)

pred = tf.nn.softmax(yo)
pred_label = tf.argmax(pred, 1)
correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(label, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

# Create operation which will initialize all variables
init = tf.global_variables_initializer()

# Configure GPU not to use all memory
config = tf.ConfigProto()
config.gpu_options.allow_growth = True

# Start a new tensorflow session and initialize variables
sess = tf.InteractiveSession(config=config)
sess.run(init)

for learning_rate in [0.05, 0.01]:
    for epoch in range(50):
        avg_cost = 0.0

        # For each epoch, we go through all the samples we have.
        for i in range(x_train.shape[0]):
            # Finally, this is where the magic happens: run our optimizer, feed the current example into X and the
            # current target into Y
            _, c = sess.run([optimizer, loss], feed_dict={lr: learning_rate,
                                                          entree: x_train[i, None],
                                                          label: labels_train[i, None]})
            avg_cost += c
        avg_cost /= x_train.shape[0]

        # Print the cost in this epcho to the console.
        if epoch % 10 == 0:
            print("Epoch: {:3d}    Train Cost: {:.4f}".format(epoch, avg_cost))

acc_train = accuracy.eval(feed_dict={entree: x_train, label: labels_train})
print("Train accuracy: {:3.2f}%".format(acc_train * 100.0))
