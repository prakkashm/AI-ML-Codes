# K NEAREST NEIGHBORS
- Data set (Training and test sets are given separately) is the OCR hand written data available at   https://sites.google.com/site/viswanathpulabaigari/data-sets
- Training data set is first shuffled and stored in 'shuffled.dat' as it was sorted on 'class label' and would otherwise result in a badly trained classifier.
- It is a 192 dimensional, 10 class problem. 
- 'k' to be used in k-nearest neighbor classifier (k-NNC) is found out using a 3-fold cross validation.
- With 'k' value thus found, k-NNC is employed and error rate on the test set is calculated.
- Refer Report.pdf for more clarity.

## Procedure to execute the program
1. Firstly, execute the file 'dist_tes.c' which will calculate and store in 'dist_tes.txt', all the euclidean distances (for test set) which will be required while executing 'knn.c'. It took 5 mins on my system for it to completely execute. It will generate a 200 mb file 'dist_tes.txt'.
2. Now, run the file 'dist_tra.c' which will calculate and store in 'dist_tra.txt', all the euclidean distances (for training set) which will be required while executing 'knn.c'. It took 10 mins on my system for it to completely execute. It will generate a 400 mb file 'dist_tra.txt'.
3. Now, run 'knn.c'. It might take 1-2 mins to give the output.
