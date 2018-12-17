import pandas as pd
import numpy as np
import tensorflow as tf

# Nombre de neurones de la couche 1
hid1_size = 6 
# Nombre de neurone de la couche 2
hid2_size = 4
# Nombre de neurone de sortie : une sortie indique 1 l'autre 0
N_OUTPUT = 2

# chargement des données
donne = pd.read_csv('./data.csv')

# Selection des données (entrée/sortie attendu)
x_in = donne.drop('res', axis=1).values

#Selection de la colonne resultat
y_in = donne['res'].values


# creation du jeu de valeur attendu en sortie 0 => [1,0], 1 => [0,1]
labels_train = (np.arange(N_OUTPUT) == y_in[:, None]).astype(np.float32)

# definitione entrée/sortie

# nombre de neurone d'entrée => nombre de valeur en entrée (16)
entree = tf.placeholder(tf.float32, name='entree', shape=(None, x_in.shape[1]))
label = tf.placeholder(tf.float32, shape=(None, N_OUTPUT), name='labels')

# tf.random_normal => création des valeurs en respectant une distribution normale
# ici moyenne =>0 (par défaut)
# ecart-type=> stddev = 0.25 (quasi toutes les valeurs dans [-1:1]

# premiere couche

w1 = tf.Variable(tf.random_normal([hid1_size, x_in.shape[1]], stddev=0.25), name='w1')
b1 = tf.Variable(tf.constant(0.1, shape=(hid1_size, 1)), name='b1')
y1 = tf.nn.sigmoid(tf.add(tf.matmul(w1, tf.transpose(entree)), b1))

#deuxième couche
w2 = tf.Variable(tf.random_normal([hid2_size, hid1_size], stddev=0.25), name='w2')
b2 = tf.Variable(tf.constant(0.1, shape=(hid2_size, 1)), name='b2')
y2 = tf.nn.sigmoid(tf.add(tf.matmul(w2, y1), b2))

#couche de sortie 
wo = tf.Variable(tf.random_normal([N_OUTPUT, hid2_size], stddev=0.25), name='wo')
bo = tf.Variable(tf.random_normal([N_OUTPUT, 1]), name='bo')
#pas de sigmoid sur la couche de sortie juste la transposé
yo = tf.transpose(tf.add(tf.matmul(wo, y2), bo))

# parametre le reseau pour qu'il minimise la moyenne de la différence entre les sorties voulues et obtenues
loss = tf.reduce_mean(tf.square(yo - label))
lr = tf.placeholder(tf.float32, shape=(), name='learning_rate')
# paramètre le reseau pour qu'il utlise la gradient pour corriger les erreurs, avec comme taux d'erreur celui donné
# par le parametre lr
optimizer = tf.train.GradientDescentOptimizer(lr).minimize(loss)

init = tf.global_variables_initializer()

pred_label = tf.argmax(yo, 1)
#On estime qu'une correction et correcte si le neurone avec la valeur de sortie le plus eleve est le bon
correct_prediction = tf.equal(pred_label, tf.argmax(label, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

sess = tf.InteractiveSession()
sess.run(init)
epoch=0
# On definit le pas d'apprentissage
for learning_rate in [0.06]:
    while epoch <100000 and accuracy.eval(feed_dict={entree: x_in, label: labels_train})<1:
        avg_cost = 0.0
        # Pour chaque etape d'apprentissage on teste toutes les valeurs d'entrés
        for i in range(x_in.shape[0]):
            # On ameliore notre model en comprant nos resultats a ceux attendus 
            _, c = sess.run([optimizer, loss], feed_dict={lr: learning_rate,
                                                          entree: x_in[i, None],
                                                          label: labels_train[i, None]})
        if epoch % 1000 == 0:
            print('Hypothesis %s' % sess.run(yo,feed_dict={entree: x_in,label:labels_train})) #On affiche l'avance de l'apprentissage
        epoch += 1


# On evalue notre RNA apres l'apprentissage t
acc_train = accuracy.eval(feed_dict={entree: x_in, label: labels_train})
print("Train accuracy: {:3.2f}%".format(acc_train * 100.0))

# Ajout une fonction derrière la couche de sortie
# Elle retourne la numero du neurone avec la valeur la plus elevée


df_test = pd.read_csv('./test.csv')
X_test = df_test.values
# On lance notre rna avec toutes nos valeurs d'entrés et on affiche le résultat de ce dernier
df_test['res'] = sess.run(pred_label, feed_dict={entree: X_test})

print(df_test)
sess.close()
