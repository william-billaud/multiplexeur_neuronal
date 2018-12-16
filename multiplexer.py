import pandas as pd
import tensorflow as tf

data = pd.read_csv('./data.csv', sep=',')

output = [[0], [1], [1], [0], [0], [1], [1], [1], [0], [0], [0], [1], [1], [1], [1], [0]]
enter = data.drop("res", axis=1)
# output = data["res"]

x_ = tf.placeholder(tf.float32, shape=[16, 4], name="x-input")
y_ = tf.placeholder(tf.float32, shape=[16, 1], name="y-input")

Theta1 = tf.Variable(tf.random_uniform([4, 16], -1, 1), name="Theta1")
Theta2 = tf.Variable(tf.random_uniform([16, 1], -1, 1), name="Theta2")

Bias1 = tf.Variable(tf.zeros([16]), name="Bias1")
Bias2 = tf.Variable(tf.zeros([1]), name="Bias2")

with tf.name_scope("layer2") as scope:
    A2 = tf.sigmoid(tf.matmul(x_, Theta1) + Bias1)

with tf.name_scope("layer3") as scope:
    Hypothesis = tf.sigmoid(tf.matmul(A2, Theta2) + Bias2)

pred = tf.nn.softmax(tf.matmul(x_, Theta1) + Bias1)  # Softma

with tf.name_scope("cost") as scope:
    cost = tf.reduce_mean(((y_ * tf.log(Hypothesis)) + ((1 - y_) * tf.log(1.0 - Hypothesis))) * -1)

with tf.name_scope("train") as scope:
    train_step = tf.train.GradientDescentOptimizer(0.01).minimize(cost)

# Construct model
logits = neural_net(X)
prediction = tf.nn.softmax(logits)

# Evaluate model
correct_pred = tf.equal(tf.argmax(prediction, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)

writer = tf.summary.FileWriter("./logs/multiplexer_logs", sess.graph_def)

for i in range(100000):
    sess.run(train_step, feed_dict={x_: enter, y_: output})
    if i % 1000 == 0:
        print('Epoch ', i)
        print('Hypothesis ', sess.run(Hypothesis, feed_dict={x_: enter, y_: output}))
        print('Theta1 ', sess.run(Theta1))
        print('Bias1 ', sess.run(Bias1))
        print('Theta2 ', sess.run(Theta2))
        print('Bias2 ', sess.run(Bias2))
        print('cost ', sess.run(cost, feed_dict={x_: enter, y_: output}))

print("Testing Accuracy:", sess.run(accuracy, feed_dict={x_: enter, y_: output}))
learned_output = tf.argmax(Hypothesis,0)
print(learned_output.eval({enter: [[0, 0, 0, 0], [0, 0, 0, 1]]}))
