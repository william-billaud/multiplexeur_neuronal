""" Neural Network.
A 2-Hidden Layers Fully Connected Neural Network (a.k.a Multilayer Perceptron)
implementation with TensorFlow. This example is using the MNIST database
of handwritten digits (http://yann.lecun.com/exdb/mnist/).
Links:
    [MNIST Dataset](htt p://yann.lecun.com/exdb/mnist/).
Author: Aymeric Damien
Project: https://github.com/aymericdamien/TensorFlow-Examples/
"""


import tensorflow as tf
import pandas as pd

data = pd.read_csv('./data.csv', sep=',')

output = [[0], [1], [1], [0], [0], [1], [1], [1], [0], [0], [0], [1], [1], [1], [1], [0]]
enter = data.drop("res", axis=1)

# Parameters
learning_rate = 0.1
num_steps = 5000
display_step = 250



# tf Graph input
X = tf.placeholder("float", [16, 4])
Y = tf.placeholder("float", [16, 1])

# Store layers weight & bias
weights = {
    'h1': tf.Variable(tf.random_normal([4, 16])),
    'h2': tf.Variable(tf.random_normal([16, 16])),
    'out': tf.Variable(tf.random_normal([16, 1]))
}
biases = {
    'b1': tf.Variable(tf.random_normal([16])),
    'b2': tf.Variable(tf.random_normal([16])),
    'out': tf.Variable(tf.random_normal([1]))
}


# Create model
def neural_net(x):
    # Hidden fully connected layer with 256 neurons
    layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    # Hidden fully connected layer with 256 neurons
    layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    # Output fully connected layer with a neuron for each class
    out_layer = tf.matmul(layer_2, weights['out']) + biases['out']
    return out_layer


# Construct model
logits = neural_net(X)
prediction = tf.nn.softmax(logits)

# Define loss and optimizer
loss_op = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(
    logits=logits, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
train_op = optimizer.minimize(loss_op)

# Evaluate model
correct_pred = tf.equal(tf.argmax(prediction, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

# Initialize the variables (i.e. assign their default value)
init = tf.global_variables_initializer()

# Start training
with tf.Session() as sess:
    # Run the initializer
    sess.run(init)

    for step in range(1, num_steps + 1):
        batch_x= enter
        batch_y = output
        # Run optimization op (backprop)
        sess.run(train_op, feed_dict={X: batch_x, Y: batch_y})
        if step % display_step == 0 or step == 1:
            # Calculate batch loss and accuracy
            loss, acc = sess.run([loss_op, accuracy], feed_dict={X: batch_x,
                                                                 Y: batch_y})
            print("Step " + str(step) + ", Minibatch Loss= " + \
                  "{:.4f}".format(loss) + ", Training Accuracy= " + \
                  "{:.3f}".format(acc))
            #print('Theta1 ', sess.run(weights['h1']))
            #print('Bias1 ', sess.run(biases['b1']))
            #print('Theta2 ', sess.run(weights['h2']))
            #print('Bias2 ', sess.run(biases['b2']))

    print("Optimization Finished!")

print("Testing Accuracy:", sess.run(accuracy, feed_dict={X: enter, Y: output}))
