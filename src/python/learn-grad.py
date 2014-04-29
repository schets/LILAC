import numpy as np
from sklearn import manifold, svm, preprocessing
import pylab as pl
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import NullFormatter
Axes3D
def read_data(gradname, scorename):
    #could use numpy.loadtxt, but that blindly accepts nan values
    slist = open(scorename, 'r').readlines()
    glist =  open(gradname, 'r').readlines()
    v = (slist, glist)
    gkeep = []
    skeep = []
    for s, g in zip(slist, glist):
        sval = float(s.split()[0])
        if(np.isfinite(sval)):
            skeep.append(np.fromstring(s, sep=' '))
            nar = np.fromstring(g, sep=' ')
            gkeep.append(nar)
    return (np.vstack(skeep), np.vstack(gkeep))


(scores, grads) = read_data("grad_data.out", "score_data.out")
(scores2, grads2) = read_data("grad_data2.out", "score_data2.out")
n_neighbors=50
n_components=15
scaler =preprocessing.StandardScaler().fit(grads);

grads = scaler.transform(grads)
grads2 = scaler.transform(grads2)
#Y = manifold.Isomap(n_neighbors, n_components).fit_transform(grads)
Y=grads #don't perform dimensionality reduction

#perform classification on the dataset, use support vector machine
test_svm = svm.SVC(kernel='rbf', C=20, gamma=0.15, probability=True)
test_svm=test_svm.fit(Y, scores[:, 1])

#get number of stable classified as unstable

pl.figure(0)
(Y, scores) = grads2, scores2
num_stab = sum(1 for s in scores if s[1] == 1)
missed_stab_arr = [s[0] for t, s, in zip(Y, scores) if s[1]==1 and test_svm.predict(t) != s[1]]
pl.hist(missed_stab_arr, bins=20)
pl.title("Frequency of scores for the misclassified stable solutions")
pl.xlabel("score")
missed_stab = len(missed_stab_arr)
ave_score = sum(missed_stab_arr)/len(missed_stab_arr);
max_score = max(missed_stab_arr)
missed_stab_arr = [s[0] for t, s, in zip(Y, scores) if s[1]==1 and test_svm.predict(t) == s[1]]
pl.figure(1)
pl.hist(missed_stab_arr, bins=50, color='green')
pl.title("Frequency of scores for the correctly classified stable solutions")
pl.xlabel("score")
ave_good_score = sum(missed_stab_arr)/len(missed_stab_arr);
max_good_score = max(missed_stab_arr)

print "%d out of %d stable solutions were missclassified" %(missed_stab, num_stab)
print "Average score of the missclassified stable variables is %f, maximum score is %f" % (ave_score, max_score)
print "Average score of the classified stable variables is %f, maximum score is %f" % (ave_good_score, max_good_score)
#calculate average probability of misclassified stable variantsdd
#calculate number of missed unstable variants
num_unstab = sum(1 for s in scores if s[1] == 0)
missed_unstab = sum(1 for t, s in zip(Y, scores) if s[1] == 0 and test_svm.predict(t) != s[1])
print "%d out of %d unstable solutions were missclassified" %(missed_unstab, num_unstab)
pl.legend()
pl.show()
"""
fig = pl.figure(figsize=(15, 8))
pl.suptitle("Manifold Learning with %i points, %i neighbors"
            % (grads.shape[1], n_neighbors), fontsize=14)

#perform clustering
# compatibility matplotlib < 1.0
ax = fig.add_subplot(241, projection='3d')
ax.scatter(Y[:, 0], Y[:, 1], Y[:, 2], c=scores[:, 0], cmap=pl.cm.Spectral)
ax.view_init(4, -72)
pl.show()
#pl.scatter(Y[:, 0], Y[:, 1], Y[:, 2], c=scores, cmap=pl.cm.Spectral)
"""
