import pandas as pd
from pandas import DataFrame
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

df = pd.read_csv('results.txt', header=None, delimiter=' ')
df.columns = ['x', 'y', 'z', 'k']
print df

threedee = plt.figure().gca(projection='3d')
threedee.scatter(df.x, df.y, df.z, c=df.k)
threedee.set_xlabel('x')
threedee.set_ylabel('y')
threedee.set_zlabel('z')
plt.show()
