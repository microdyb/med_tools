#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkHessianToObjectnessMeasureImageFilter.h"
#include "itkMultiScaleHessianBasedMeasureImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include <iostream>

int main( int argc, char *argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName> <OutputFileName> <ScalesOutputImage>";
    std::cerr << " [SigmaMinimum] [SigmaMaximum]";
    std::cerr << " [NumberOfSigmaSteps]";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];
  double sigmaMinimum = 5.0;
  if( argc > 4 )
    {
    sigmaMinimum = atof( argv[4] );
    }
  double sigmaMaximum = 10.0;
  if( argc > 5 )
    {
    sigmaMaximum = atof( argv[5] );
    }
  unsigned int numberOfSigmaSteps = 10;
  if( argc > 6 )
    {
    numberOfSigmaSteps = atoi( argv[6] );
    }
	//std::cout <<sigmaMinimum<<" "<<sigmaMaximum<<" "<<numberOfSigmaSteps<< std::endl;

  const unsigned int Dimension       = 3;
  typedef float     InputPixelType;
  typedef float      OutputPixelType;
  typedef itk::Image< InputPixelType, Dimension >  InputImageType;
  typedef itk::ImageFileReader< InputImageType >         ReaderType;

  ReaderType::Pointer   reader = ReaderType::New();
  reader->SetFileName( inputFileName ); 
  /*try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }*/

  typedef itk::SymmetricSecondRankTensor< double, Dimension > HessianPixelType;
  typedef itk::Image< HessianPixelType, Dimension >           HessianImageType;
  typedef itk::HessianToObjectnessMeasureImageFilter< HessianImageType, InputImageType >
    ObjectnessFilterType;
  ObjectnessFilterType::Pointer objectnessFilter = ObjectnessFilterType::New();
  objectnessFilter->SetBrightObject( true );
  objectnessFilter->SetScaleObjectnessMeasure( false );
	objectnessFilter->SetObjectDimension(1);
  objectnessFilter->SetAlpha(0.5);
  objectnessFilter->SetBeta(0.5);
  objectnessFilter->SetGamma(5.0);

  typedef itk::MultiScaleHessianBasedMeasureImageFilter< InputImageType, HessianImageType, InputImageType >
    MultiScaleEnhancementFilterType;
  MultiScaleEnhancementFilterType::Pointer multiScaleEnhancementFilter =
    MultiScaleEnhancementFilterType::New();
  multiScaleEnhancementFilter->SetInput( reader->GetOutput() );
  multiScaleEnhancementFilter->SetHessianToMeasureFilter( objectnessFilter );
  multiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
  multiScaleEnhancementFilter->SetSigmaMinimum( sigmaMinimum );
  multiScaleEnhancementFilter->SetSigmaMaximum( sigmaMaximum );
  multiScaleEnhancementFilter->SetNumberOfSigmaSteps( numberOfSigmaSteps );

 try
    {
    multiScaleEnhancementFilter->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::RescaleIntensityImageFilter< InputImageType, OutputImageType >
    RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput( multiScaleEnhancementFilter->GetOutput() );

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputFileName );
  writer->SetInput( rescaleFilter->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

	RescaleFilterType::Pointer rescaleFilter2 = RescaleFilterType::New();
  rescaleFilter2->SetInput( multiScaleEnhancementFilter->GetScalesOutput() );
	rescaleFilter2->SetOutputMaximum(1.0);
	rescaleFilter2->SetOutputMinimum(0);

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(argv[3]);
  writer2->UseCompressionOn();
  writer2->SetInput( rescaleFilter2->GetOutput() );

 try
    {
    writer2->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
