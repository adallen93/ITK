/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    QPropXORTest1.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkOneHiddenLayerBackPropagationNeuralNetwork.h"
#include "itkIterativeSupervisedTrainingFunction.h"
#include "itkBatchSupervisedTrainingFunction.h"
#include "itkVector.h"
#include "itkListSample.h"
#include <vector>
#include <fstream>
#include <time.h>

#define ROUND(x) (floor(x+0.5))

int
QPropXORTest1(int argc, char* argv[])
{
  if (argc < 1)
    {
    std::cout << "ERROR: data file name argument missing." << std::endl ;
    return EXIT_FAILURE;
    }

  char* dataFileName =argv[1]; //"qpropxortest.txt";
  const int num_input_nodes = 2;
  const int num_hidden_nodes = 2;
  const int num_output_nodes = 1;

  srand(time(0)); 
  typedef itk::Vector<double, num_input_nodes> MeasurementVectorType;
  typedef itk::Vector<double, num_output_nodes> TargetVectorType;
  typedef itk::Statistics::ListSample<MeasurementVectorType> SampleType;
  typedef itk::Statistics::ListSample<TargetVectorType> TargetType;

  typedef itk::Statistics::BatchSupervisedTrainingFunction<SampleType, TargetType, double> TrainingFcnType;

  MeasurementVectorType mv;
  TargetVectorType tv;
  SampleType::Pointer sample = SampleType::New();
  TargetType::Pointer targets = TargetType::New();
  sample->SetMeasurementVectorSize( num_input_nodes);
  targets->SetMeasurementVectorSize( num_output_nodes);
 
  std::ifstream infile1;
  infile1.open(dataFileName, std::ios::in);

  infile1 >> mv[0] >> mv[1] >> tv[0];

  while (!infile1.eof())
    {
    std::cout << "Input =" << mv << std::endl;
    std::cout << "target =" << tv << std::endl;
    sample->PushBack(mv);
    targets->PushBack(tv);
    infile1 >> mv[0] >> mv[1] >> tv[0];
    }
  infile1.close();

  std::cout << sample->Size() << std::endl;

  typedef itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork
                                                   <MeasurementVectorType,
                                                   TargetVectorType> NetworkType;
  typedef NetworkType::LayerType LayerType;

  NetworkType::Pointer net1 = NetworkType::New();
  net1->SetNumOfInputNodes(num_input_nodes);
  net1->SetNumOfHiddenNodes(num_hidden_nodes);
  net1->SetNumOfOutputNodes(num_output_nodes);
  
  typedef itk::Statistics::SymmetricSigmoidTransferFunction<double> tfType;
  tfType::Pointer transferfunction1=tfType::New();
  net1->SetHiddenTransferFunction(transferfunction1);
  net1->SetOutputTransferFunction(transferfunction1);
  
  typedef itk::Statistics::QuickPropLearningRule<LayerType,
                                         TargetVectorType> LearningFunctionType;
  LearningFunctionType::Pointer learningfunction=LearningFunctionType::New();

  net1->SetLearningFunction(learningfunction);

  net1->SetHiddenLayerBias(1.0);
  net1->SetOutputLayerBias(1.0);
  
  net1->Initialize();

  TrainingFcnType::Pointer trainingfcn = TrainingFcnType::New();
  trainingfcn->SetIterations(50);
  
  trainingfcn->SetThreshold(0.001); 
  
  //Network Simulation
  std::cout << sample->Size() << std::endl;
  std::cout << "Network Simulation" << std::endl;
  TargetVectorType ov;
  SampleType::ConstIterator iter1 = sample->Begin();
  TargetType::ConstIterator iter2 = targets->Begin();


  unsigned int error1 = 0 ;
  unsigned int error2 = 0 ;
  int flag = 0;
  int train_flag=1;
  long num_iterations =0;
  long max_iterations=1000;

  std::ofstream outfile;
  outfile.open("out1.txt",std::ios::out);
  
  while (train_flag==1)
    {
    //train the network
    net1->InitializeWeights();
    trainingfcn->Train(net1, sample, targets);
    num_iterations+=50;   
    iter1 = sample->Begin();
    iter2 = targets->Begin();
    error1=0;
    error2=0;
    //Simulate the network
    while (iter1 != sample->End())
      {
      mv = iter1.GetMeasurementVector();
      tv = iter2.GetMeasurementVector();
      ov = net1->GenerateOutput(mv);
      outfile<<mv[0]<<" "<<mv[1]<<" "<<tv[0]<<" "<<ov[0]<<std::endl;
      std::cout << "Network Input = " << mv << std::endl;
      std::cout << "Network Output = " << ov << std::endl;
      std::cout << "Target = " << tv << std::endl;
      flag = 0;
      std::cout<<fabs(tv[0]-ov[0])<<std::endl;
      if (fabs(tv[0]-ov[0])>0.2)
        {
        flag = 1;
        }
      if (flag == 1 && tv[0] == 0.5)
        {
        ++error1;
        }
      else if (flag == 1 && tv[0] == -0.5)
        {
        ++error2;
        }
      ++iter1;
      ++iter2;
      }
    //check for convergence
    if((error1+error2)==0 || num_iterations > max_iterations)
      {//Done training
      train_flag=0;
      }
    }//outer while

  std::cout<<"Number of Epochs = "<<num_iterations<<std::endl;
  std::cout << "Among 4 measurement vectors, " << error1 + error2
            << " vectors are misclassified." << std::endl ;
  std::cout<<"Network Weights and Biases after Training= "<<std::endl;
   
  std::cout << net1 << std::endl;
  
  if ((error1 + error2) > 2)
    {
    std::cout << "Test failed." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}
