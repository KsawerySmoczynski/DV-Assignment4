//
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute in modified or unmodified form! 
// Copyright (C) 2018 CGV TU Dresden - All Rights Reserved
//

#include <vtkAutoInit.h>
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingFreeType );

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkDEMReader.h>
#include <vtkContourFilter.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkWarpScalar.h>
#include <vtkDataSetMapper.h>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRendererCollection.h>

#include <vtkColorTransferFunction.h>
#include <vtkScalarBarActor.h>
#include <vtkTextRenderer.h>

// standard includes
#include <vector>
#include <algorithm>




// ----- utility functions -----
void setGradientBackground( vtkSmartPointer<vtkRenderer> renderer )
{
	renderer->GradientBackgroundOn();
	renderer->SetBackground( 0.5, 0.5, 0.5 );
	renderer->SetBackground2( 0.9, 0.9, 0.9 );
}
// ----- end of utility functions -----


vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper( vtkSmartPointer<vtkMapper> mapper )
{
	//create renderer, window and actor
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	
	// let's have a fancy background for the canvas
	setGradientBackground( renderer );
	// fallback color, is used whenever mappers do not utilize scalars for coloring
	actor->GetProperty()->SetColor( 1, 1, 1 );

	// connect everything
	actor->SetMapper( mapper );
	renderer->AddActor( actor );
	window->AddRenderer( renderer );
	
	// set window size and return
	window->SetSize( 800, 800 );
	return window;
}

vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMultipleMappers( std::vector<vtkSmartPointer<vtkMapper>> mappers )
{
	// create renderer and window
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();

	// let's have a fancy background for the canvas
	setGradientBackground( renderer );

	// connect window and renderer
	window->AddRenderer( renderer );

	// task 4.2
	// for every mapper we create an actor and add it to the renderer
	// (do not forget to set the fallback color (see createRenderWindowFromMapper(...) )
	
	// student begin...

	// your code here

	// ... student end


	// set window size and return
	window->SetSize( 800, 800 );
	return window;
}

void doRenderingAndInteraction( vtkSmartPointer<vtkRenderWindow> window )
{
    // create interactor and connect a window
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( window );
	// set an interaction style
	interactor->SetInteractorStyle( vtkInteractorStyleTrackballCamera::New() );

    // execute render/interaction loop
    interactor->Initialize();
    interactor->Start();

    // close the window when finished
    window->Finalize();
}


int main(int argc, char * argv[])
{
    // -- begin of basic visualization network definition --
   
	// 1. create source
	vtkSmartPointer<vtkDEMReader> source = vtkSmartPointer<vtkDEMReader>::New();
	source->SetFileName( "../data/SainteHelens.dem" );
	
    // 2. create filters
	// a) contour filter
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	// use source as filter input
	contourFilter->SetInputConnection( source->GetOutputPort() );
	// set the height value at which the contour line shall be drawn
	contourFilter->SetValue( 0, 1800 );
	
	//b) gradient filter
	vtkSmartPointer<vtkImageGradientMagnitude> gradientFilter = vtkSmartPointer<vtkImageGradientMagnitude>::New();
	// how many dimensions does the data have
	gradientFilter->SetDimensionality( 2 );
	// use source as filter input
	gradientFilter->SetInputConnection( source->GetOutputPort() );
	

	// 3.  create mappers
	// a) image mapper, show the data as 2D image with standard color transfer function
	vtkSmartPointer<vtkDataSetMapper> imageMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	// connect directly to source (the pipeline is source->imageMapper->...)
	imageMapper->SetInputConnection( source->GetOutputPort() );
	// set the range of the scalar values in order to correctly map the colors
	imageMapper->SetScalarRange( 682, 2543 );
	// b) contour mapper, show the regions where the data has a specific value
	vtkSmartPointer<vtkDataSetMapper> contourMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	// connect to the contour filter output (the pipeline is source->contourFilter->contourMapper->...)
	contourMapper->SetInputConnection( contourFilter->GetOutputPort() );
	// avoid z-buffer fighting with small polygon shift
	contourMapper->SetResolveCoincidentTopologyToPolygonOffset();
	// don't use the scalar value to color the line, see fallback coloring via actor in createRenderWindowFromMapper
	contourMapper->ScalarVisibilityOff();
	
	// c) gradient mapper, show the gradient magnitudes as 2D image 
	vtkSmartPointer<vtkDataSetMapper> gradientMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	// connect to the gradient filter output (the pipeline is source->gradientFilter->gradientMapper->...)
	gradientMapper->SetInputConnection( gradientFilter->GetOutputPort() );
	// set the range of the gradient magnitudes in order to correctly map the colors
	gradientMapper->SetScalarRange( 0, 2 );
	
    // -- end of basic visualization network definition --


    // 4. create actors, renderers, render windows 	
	vtkSmartPointer<vtkRenderWindow> imageWindow = createRenderWindowFromMapper( imageMapper );
	vtkSmartPointer<vtkRenderWindow> contourWindow = createRenderWindowFromMapper( contourMapper );
	vtkSmartPointer<vtkRenderWindow> gradientWindow = createRenderWindowFromMapper( gradientMapper );
	
    // 5. successively show each window and allow user interaction (until it is closed)
	doRenderingAndInteraction( imageWindow );
	doRenderingAndInteraction( contourWindow );
	doRenderingAndInteraction( gradientWindow );

    return 0;
}