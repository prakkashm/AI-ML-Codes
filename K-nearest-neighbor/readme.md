# K NEAREST NEIGHBORS
- Data set (Training and test sets are given separately) is the OCR hand written data available at   https://sites.google.com/site/viswanathpulabaigari/data-sets
- Training data set is first shuffled and stored in 'shuffled.dat' as it was sorted on 'class label' and would otherwise result in a badly trained classifier.
- It is a 192 dimensional, 10 class problem. 
- 'k' to be used in k-nearest neighbor classifier (k-NNC) is found out using a 3-fold cross validation.
- With 'k' value thus found, k-NNC is employed and error rate on the test set is calculated.
- Refer Report.pdf for more clarity.
