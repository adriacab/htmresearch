/* ---------------------------------------------------------------------
 * Numenta Platform for Intelligent Computing (NuPIC)
 * Copyright (C) 2014-2015, Numenta, Inc.  Unless you have an agreement
 * with Numenta, Inc., for a separate license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * http://numenta.org/licenses/
 * ----------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------
 * This file runs the MNIST dataset using a simple model composed of a
 * set of dendrites. Each dendrite randomly samples pixels from one image.
 *
 * Key parameters:
 *     Binarization threshold
 *     Number of synapses per dendrite
 *     Threshold
 *     min threshold
 * ----------------------------------------------------------------------
 */

#include <assert.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include <nupic/math/Math.hpp>
#include <nupic/math/SparseMatrix.hpp>
#include <nupic/math/SparseMatrix01.hpp>
#include <nupic/types/Types.hpp>
#include <nupic/utils/Random.hpp>

#include "dendrite_classifier1.hpp"
#include "dendrite_classifier2.hpp"
#include "knn_classifier.hpp"

using namespace std;
using namespace nupic;

///////////////////////////////////////////////////////////
//
// External definitions, should really be in header files

// Read test/training images from top-level directory and return number read
extern int readImages(int *numImages, const char *path,
                std::vector< SparseMatrix01<UInt, Int> * > &images,
                Real samplingFactor = 1.0);

extern void classifyDataset(
           int threshold,
           std::vector< SparseMatrix01<UInt, Int> * > &dataSet,
           std::vector< SparseMatrix01<UInt, Int> * > &dendrites);

extern void trainDendrites(int k, int nSynapses,
           std::vector< SparseMatrix01<UInt, Int> * > &trainingSet,
           std::vector< SparseMatrix01<UInt, Int> * > &dendrites,
           Random &r);

extern void trainDendrites2(int k, int nSynapses,
           std::vector< SparseMatrix01<UInt, Int> * > &trainingSet,
           std::vector< SparseMatrix01<UInt, Int> * > &dendrites,
           Random &r);


// Run the whole MNIST example.
void runMNIST(int nSynapses, int trainingThreshold, Real samplingFactor)
{
  //////////////////////////////////////////////////////
  //
  // Initialize the sparse matrix data structures. Our classifier will be a set
  // of dendrites. dendrites[k] will contain a set of dendrites trained on
  // class k.
  std::vector< SparseMatrix01<UInt, Int> * > trainingSet;
  std::vector< SparseMatrix01<UInt, Int> * > testSet;

  for (int i= 0; i<10; i++)
  {
    trainingSet.push_back( new SparseMatrix01<UInt, Int>(28*28, 1));
    testSet.push_back( new SparseMatrix01<UInt, Int>(28*28, 1));
  }

  DendriteClassifier1 model1(1000);
  DendriteClassifier model2;
  KNNClassifier knnModel;

  //////////////////////////////////////////////////////
  //
  // Read in the given number of training and test images
  int trainingImages[] = {
    5923, 6742, 5958, 6131, 5842, 5421, 5918, 6265, 5851, 5949
  };
  int testImages[] = { 980, 1135, 1032, 1010, 982, 892, 958, 1028, 974, 1009 };

  int numImages = readImages(trainingImages,
             "../image_test/mnist_extraction_source/training/%d", trainingSet,
             samplingFactor);
  cout << "Read in " << numImages << " total images\n";

  int numTestImages = readImages(testImages,
             "../image_test/mnist_extraction_source/testing/%d", testSet);
  cout << "Read in " << numTestImages << " total test images\n";


  //////////////////////////////////////////////////////
  //
  // Create trained model for each category, by randomly sampling from
  // training images.
  cout << "Training KNN model.\n";
  knnModel.trainDataset(trainingSet);
//  cout << "Training dendrite model1 with " << nSynapses
//       << " synapses per dendrite.\n";
//  model1.trainDataset(nSynapses, trainingSet);
//  cout << "Training dendrite model2 with " << nSynapses
//       << " synapses per dendrite.\n";
//  model2.trainDataset(nSynapses, trainingThreshold, trainingSet);
//
//
//  //////////////////////////////////////////////////////
//  //
//  // Classify the data sets and compute accuracy
//  cout << "Running classification with a bunch of different thresholds.\n";
//  for (int threshold = 36; threshold <= 40; threshold+= 2)
//  {
//    cout << "\nUsing threshold = " << threshold << "\n";
//    cout << "Training set using model1:\n";
//    model1.classifyDataset(threshold, trainingSet);
//    cout << "Training set using model2:\n";
//    model2.classifyDataset(threshold, trainingSet);
//
//    cout << "\nTest set: \n";
////    model2.classifyDataset(threshold, testSet);
//    model1.classifyDataset(threshold, testSet);
//  }

}


// Run the trials!  Currently need to hard code the specific trial you are
// about to run.
int main(int argc, char * argv[])
{
  runMNIST(40, 38, 0.01);
}

