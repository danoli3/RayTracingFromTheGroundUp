
// this file contains the definition of the World class

//#include "wxraytracer.h"

#include "World.h"
#include "Constants.h"

// geometric objects
//#include "Compound.h"
//#include "RegularGrid.h"
#include "Plane.h"
#include "Sphere.h"
//#include "ConcaveSphere.h"
//#include "ConcaveCylinder.h"
//#include "ConvexPartCylinder.h"
//#include "ConcavePartCylinder.h"
//#include "ConcavePartSphere.h"
//#include "OpenCylinder.h"
//#include "SingleSphere.h"
//#include "Box.h"
//#include "Rectangle.h"
//#include "Instance.h"
//#include "Annulus.h"
//#include "ConcaveHemisphere.h"
//#include "ConcaveCylinder.h"
//#include "ConvexCylinder.h"


// tracers

//#include "SingleSphere.h"
//#include "MultipleObjects.h"
#include "RayCast.h"
//#include "Whitted.h"
//#include "AreaLighting.h"
//#include "PathTrace.h"

// cameras

#include "Pinhole.h"
//#include "ThinLens.h"
//#include "Fisheye.h"
//#include "Spherical.h"
//#include "StereoCamera.h"
//#include "ThirdEye.h"

// Textures
//#include "Texture.h"
//#include "Checker3D.h"
//#include "ImageTexture.h"

// Mappings
//#include "Mapping.h"
//#include "SphericalMap.h"
//#include "RectangularMap.h"
//#include "CylindricalMap.h"
//#include "HemisphericalMap.h"


//#include "Mesh.h"
//#include "ply.h"
//#include "Triangle.h"
//#include "TriangleMesh.h"
//#include "MeshTriangle.h"
//#include "MeshData.h"
//#include "FlatUVMeshTriangle.h"
//#include "SmoothTriangle.h"
//#include "SmoothUVMeshTriangle.h"
//#include "SmoothMeshTriangle.h"


// lights

#include "Directional.h"
//#include "PointLight.h"
//#include "AmbientOccluder.h"
//#include "AreaLight.h"
//#include "EnvironmentLight.h"

// materials

#include "Material.h"
//#include "Matte.h"
//#include "SV_Matte.h"
//#include "SV_Emissive.h"
//#include "Phong.h"
//#include "Reflective.h"
//#include "Emissive.h"
//#include "Transparent.h"
//#include "Dielectric.h"

// utilities

//#include "BBox.h"
#include "Vector3D.h"
#include "Point2D.h"
#include "Point3D.h"
#include "Normal.h"
#include "ShadeRec.h"
#include "RGBColor.h"
//#include "Fibonacci.h"
//#include "Fractal.h"
//#include "Image.h"

#include <math.h>

namespace RayTracer {
    

void
World::build(void) {
  
    
}


// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
	:  	background_color(black),
		tracer_ptr(NULL),
		ambient_ptr(new Ambient),
		camera_ptr(NULL),
		random(new RandomNumber())//,
//		photontracer_ptr(NULL),
//		photonmap(NULL)
{
	
	
}

//------------------------------------------------------------------ destructor
World::~World(void) {	
	
	if(tracer_ptr) {
		delete tracer_ptr;
		tracer_ptr = NULL;
	}
	
		
	if (ambient_ptr) {
		delete ambient_ptr;
		ambient_ptr = NULL;
	}
			
		
	if (camera_ptr) {
		delete camera_ptr;
		camera_ptr = NULL;
	}
//
//	if (photontracer_ptr) {
//		delete photontracer_ptr;
//		photontracer_ptr = NULL;
//	}
//	



	if (random) {
		delete random;
		random = NULL;
	}
	
	delete_objects();	
	delete_lights();	
	delete_images();
}

//------------------------------------------------------------------ render_scene
void 												
World::render_scene(void) const {

	RayTracer::RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 100.0;				// hardwired in

	ray.d = Vector3D(0, 0, -1);
	
	for (int r = 0; r < vres; r++)			// up
		for (int c = 0; c <= hres; c++) {	// across 					
			ray.o = Point3D(s * (c - hres / 2.0 + 0.5), s * (r - vres / 2.0 + 0.5), zw);
			pixel_color = tracer_ptr->trace_ray(ray);
			display_pixel(r, c, pixel_color);
		}	
}  

// This uses orthographic viewing along the zw axis

void 												
World::render_scene(const std::vector<Pixel>& pixels) const {

//	RayTracer::RGBColor	pixel_color;	 	
//	Ray			ray;					
//	int 		hres 	= vp.hres;
//	int 		vres 	= vp.vres;
//	float		s		= vp.s;
//	float		zw		= 100.0;				// hardwired in
//	
//	list<RenderedPixel> render;   // for send every row
//	RenderedPixel pixel;		  // "
//	int count = 0;
//	int jump  = 0;
//	int depth = 0;
//
//	ray.d = Vector3D(0, 0, -1);
//	
//	for(int i = 0; i< pixels.size(); i++)
//	{			
//			Pixel screen_pixel = pixels[i];		
//			ray.o = Point3D(s * (screen_pixel.x - hres / 2.0 + 0.5), s * (screen_pixel.y - vres / 2.0 + 0.5), zw);
//			pixel_color = tracer_ptr->trace_ray(ray, depth, count, jump);
//			pixel.color = pixel_color;			// for send every row
//			pixel.xy = Point2D(screen_pixel.x,screen_pixel.y);	// "
//			render.push_back(pixel);    // "
//
//				if(stop_rendering())       // if the program is asked to close, we need end this now
//				{	display_pixel(render);  
//					render.clear();	
//					return; 	}	
//
//				if(render_display() == EVERY_PIXEL)
//				{	display_pixel(render);   // send to the screen buffer every pixel rendered
//					render.clear();	
//				}
//				else if(render_display() == EVERY_ROW)
//				{	
//					if(i % (pixels.size()/10) == 0)
//					{
//						display_pixel(render);   // send to the screen buffer every pixel rendered
//						render.clear();	
//					}
//				}
//	}	
//	if(render_display() == EVERY_JOB || render_display() == EVERY_ROW)
//	{	display_pixel(render);   // send to the screen buffer every row of pixels rendered
//		render.clear();	
//	}		

	
}

void
World::render_scene(const Pixel& pixel, RenderedPixel & rendered) const {
	
	RayTracer::RGBColor	pixel_color = black;
	Vector3D u(1, 0, 0);
	Vector3D v(0, 1, 0);
	Vector3D w(0, 0, 1);
	
	float exposure_time = 1.0f;
	
	Ray			ray;
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
    //if(camera_ptr)
	//	s 		/= (static_cast<Pinhole*>(camera_ptr)->zoom);
	s = -0.333;
	float		zw		= 1000.0;
	
	Point2D 	pp;			// sample point on a pixel
    int n = (int)sqrt((float)vp.num_samples);
    n = 1;
	
    rendered.color = RayTracer::RGBColor();
	int count = 0;
	int jump  = 0;
	int depth = 0;
	
	ray.d = Vector3D(0, 0, -1);
	
	ray.o = Point3D(s * (pixel.x - hres / 2.0 + 0.5), s * (pixel.y - vres / 2.0 + 0.5), zw);
	
	int sp_count = 0;
	int sp_jump = 0;
	for (int p = 0; p < n; p++)	{		// up pixel
		for (int q = 0; q < n; q++) {	// across pixel
			pp.x = vp.s * (pixel.x - 0.5 * vp.hres + (q + 0.5) / n);
			pp.y = vp.s * (pixel.y - 0.5 * vp.vres + (p + 0.5) / n);
			
			Vector3D dir = pp.x * u + pp.y * v - zw * w;
			dir.normalize();
			ray.d = dir;
			
			pixel_color += tracer_ptr->trace_ray(ray, depth, count, jump);
		}
	}
	
	//pixel_color = tracer_ptr->trace_ray(ray, depth, count, jump);
	
	
	pixel_color /= vp.num_samples;
	pixel_color *= exposure_time;
	
	rendered.color = pixel_color;			// for send every row
	rendered.xy = Point2D(pixel.x,pixel.y);
	
	
	return;
	
	
	
}


// ------------------------------------------------------------------ clamp

RayTracer::RGBColor
World::max_to_one(const RayTracer::RGBColor& c) const  {
	float max_value = max(c.r, max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}

// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RayTracer::RGBColor
World::clamp_to_color(const RayTracer::RGBColor& raw_color) const {
	RayTracer::RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}		
	return (c);
}

// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function


void
World::display_pixel(const int row, const int column, const RayTracer::RGBColor& raw_color) const {
//	RayTracer::RGBColor mapped_color;
//
//	if (vp.show_out_of_gamut)
//		mapped_color = clamp_to_color(raw_color);
//	else
//		mapped_color = max_to_one(raw_color);
//	
//	if (vp.gamma != 1.0)
//		mapped_color = mapped_color.powc(vp.inv_gamma);
//	
//   //have to start from max y coordinate to convert to screen coordinates
//   int x = column;
//   int y = vp.vres - row - 1;
//
//   paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
//                             (int)(mapped_color.g * 255),
//                             (int)(mapped_color.b * 255));
}


void
World::display_pixel(const list<RenderedPixel>& render) const {
	
//	list<RenderedPixel>::const_iterator it;	
//	//list<RenderedInt> rendered;
//	//vector<RenderedInt> rendered;
//	vector<RenderPixel*> rendered;
//
//	rendered.reserve(render.size());
//	for ( it=render.begin() ; it != render.end(); it++)
//	{   
//		RayTracer::RGBColor mapped_color;
//
//		if (vp.show_out_of_gamut)
//			mapped_color = clamp_to_color(it->color);
//		else
//			mapped_color = max_to_one(it->color);
//	
//		if (vp.gamma != 1.0)
//			mapped_color = mapped_color.powc(vp.inv_gamma);
//
//	  
//	
//	   //have to start from max y coordinate to convert to screen coordinates
//	   //int x = it->xy.x;
//	   int y = vp.vres - it->xy.y - 1;
	   /*RenderedInt rend;
	   rend.x = it->xy.x;
	   rend.y = y;
	   rend.r = (int)(mapped_color.r * 255);
       rend.g = (int)(mapped_color.g * 255);
       rend.b = (int)(mapped_color.b * 255);
//	   rendered.push_back(rend);*/
//
//	   rendered.push_back(new RenderPixel);
//	   rendered.back()->x = it->xy.x;
//	   rendered.back()->y = y;
//	   rendered.back()->red = (int)(mapped_color.r * 255);
//       rendered.back()->green = (int)(mapped_color.g * 255);
//       rendered.back()->blue = (int)(mapped_color.b * 255);	 
//	}
//	
//   paintArea->setPixel(rendered);
//   rendered.clear();
}



// ----------------------------------------------------------------------------- hit_objects
ShadeRec									
World::hit_objects(const Ray& ray) {

	ShadeRec	sr(*this);
	sr.count = 0;
	sr.jump = 0;
	double		t;
	Normal normal;
	Point3D local_hit_point;
	double		tmin 			= HUGE_VALUE;
	int 		num_objects 	= objects.size();
	
	
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object	= true;
			tmin 				= t;
			if(objects[j]->get_material())
				sr.material_ptr     = objects[j]->get_material();
			sr.hit_point 		= ray.o + t * ray.d;
			normal 				= sr.normal;
			local_hit_point	 	= sr.local_hit_point;
		}  
	if(sr.hit_an_object) {
		sr.t = tmin;
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}		
	return(sr);   
}

//------------------------------------------------------------------ delete_objects
// Deletes the objects in the objects array, and erases the array.
// The objects array still exists, because it's an automatic variable, but it's empty 
void
World::delete_objects(void) {
	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {

		//objects[j].Delete();

		//delete objects[j];		
		//objects[j] = NULL;
	}		
	objects.erase (objects.begin(), objects.end());

}

//------------------------------------------------------------------ delete_lights
void
World::delete_lights(void) {
	int num_lights = lights.size();
	
	for (int j = 0; j < num_lights; j++) {		
		//lights[j].Delete();

		//delete lights[j];
		//lights[j] = NULL;
	}	
	
	lights.erase (lights.begin(), lights.end());
}

void
World::delete_images(void) {
	int num_images = images.size();
	
	for (int j = 0; j < num_images; j++) {
		delete images[j];
		images[j] = NULL;
	}		
	images.clear();
}

//void World::generate_photonmap()
//{
//	photontracer_ptr->generate_photon_map();
//}

//bool
//World::stop_rendering() const
//{
//	if(paintArea != NULL)
//		return paintArea->Stop();
//	else
//		return true;
//}
//
//RenderDisplay
//World::render_display() const
//{
//	if(paintArea != NULL)
//		return paintArea->Display();
//	else
//		return EVERY_ROW;
//}

}
