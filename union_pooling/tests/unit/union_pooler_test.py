# ----------------------------------------------------------------------
# Numenta Platform for Intelligent Computing (NuPIC)
# Copyright (C) 2015, Numenta, Inc.  Unless you have an agreement
# with Numenta, Inc., for a separate license for this software code, the
# following terms and conditions apply:
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses.
#
# http://numenta.org/licenses/
# ----------------------------------------------------------------------


import unittest

import numpy

from union_pooling.union_pooler import UnionPooler



REAL_DTYPE = numpy.float32



class UnionPoolerTest(unittest.TestCase):


  def setUp(self):
    self.unionPooler = UnionPooler(inputDimensions=5,
                                   columnDimensions=5,
                                   potentialRadius=16,
                                   potentialPct=0.9,
                                   globalInhibition=True,
                                   localAreaDensity=-1.0,
                                   numActiveColumnsPerInhArea=2.0,
                                   stimulusThreshold=2,
                                   synPermInactiveDec=0.01,
                                   synPermActiveInc=0.03,
                                   synPermConnected=0.3,
                                   minPctOverlapDutyCycle=0.001,
                                   minPctActiveDutyCycle=0.001,
                                   dutyCyclePeriod=1000,
                                   maxBoost=1.0,
                                   seed=42,
                                   spVerbosity=0,
                                   wrapAround=True,

                                   # union_pooler.py parameters
                                   activeOverlapWeight=1.0,
                                   predictedActiveOverlapWeight=10.0,
                                   maxUnionActivity=0.20,
                                   exciteFunctionType='Fixed',
                                   decayFunctionType='NoDecay')


  def testDecayPoolingActivationDefaultDecayRate(self):
    self.unionPooler._poolingActivation = numpy.array([0, 1, 2, 3, 4],
                                                      dtype=REAL_DTYPE)
    expected = numpy.array([0, 1, 2, 3, 4], dtype=REAL_DTYPE)

    result = self.unionPooler._decayPoolingActivation()
    print result
    self.assertTrue(numpy.array_equal(expected, result))


  def testAddToPoolingActivation(self):
    activeCells = numpy.array([1, 3, 4])

    overlaps = numpy.array([0.123, 0.0, 0.0, 0.456, 0.789])
    expected = [0.0, 10.0, 0.0, 10.0, 10.0]

    result = self.unionPooler._addToPoolingActivation(activeCells, overlaps)
    self.assertTrue(numpy.allclose(expected, result))


  def testAddToPoolingActivationExistingActivation(self):
    self.unionPooler._poolingActivation = numpy.array([0, 1, 2, 3, 4],
                                                      dtype=REAL_DTYPE)
    activeCells = numpy.array([1, 3, 4])
    #                      [    0,   1,   0,     1,     1]
    overlaps = numpy.array([0.123, 0.0, 0.0, 0.456, 0.789])
    expected = [0.0, 11.0, 2.0, 13, 14]

    result = self.unionPooler._addToPoolingActivation(activeCells, overlaps)
    self.assertTrue(numpy.allclose(expected, result))


  def testGetMostActiveCellsUnionSizeZero(self):
    self.unionPooler._poolingActivation = numpy.array([0, 1, 2, 3, 4],
                                                      dtype=REAL_DTYPE)
    self.unionPooler._maxUnionCells = 0

    result = self.unionPooler._getMostActiveCells()

    self.assertEquals(len(result), 0)


  def testGetMostActiveCellsRegular(self):
    self.unionPooler._poolingActivation = numpy.array([0, 1, 2, 3, 4],
                                                      dtype=REAL_DTYPE)

    result = self.unionPooler._getMostActiveCells()

    self.assertEquals(len(result), 1)
    self.assertEquals(result[0], 4)


  def testGetMostActiveCellsIgnoreZeros(self):
    self.unionPooler._poolingActivation = numpy.array([0, 0, 0, 3, 4],
                                                      dtype=REAL_DTYPE)
    self.unionPooler._maxUnionCells = 3

    result = self.unionPooler._getMostActiveCells()

    self.assertEquals(len(result), 2)
    self.assertEquals(result[0], 3)
    self.assertEquals(result[1], 4)


if __name__ == "__main__":
  unittest.main()
