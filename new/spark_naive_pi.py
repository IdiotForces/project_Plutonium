
import sys
import operator
import pyspark

if __name__ == '__main__':

    time_iteration = (len(sys.argv) >= 2) and int(sys.argv[1]) or 10000

    spark_conf = pyspark.SparkConf().setAppName("NaivePi").set("spark.mesos.coarse", "true")
    sc = pyspark.SparkContext(conf=spark_conf)

    n_array = sc.parallelize(xrange(0, time_iteration), 18)
    ret = n_array.map(lambda n: pow(-1, n) * (1.0 / (2*n+1))).reduce(operator.add)
    print('pi: %.8f' % (ret*4))

