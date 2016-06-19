import numpy as np
from sklearn.datasets.samples_generator import make_blobs
X, y = make_blobs(n_samples=10000, centers=10, n_features=3, random_state=42)
print "X(rows, columns): ", X.shape
np.savetxt("input.txt", np.asarray(X), delimiter=" ")
