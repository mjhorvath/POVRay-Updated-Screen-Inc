// This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
// letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

//               Persistence of Vision Raytracer Version 3.6
//                           Screen Include File

// Created by Christoph Hormann, Chris Huff, Rune S. Johansen and Michael 
// Horvath.

// Screen.inc will enable you to place objects and textures right in front
// of the camera. One use of this is to place your signature or a logo in
// the corner of the image.

// You can only use screen.inc with the perspective camera. Screen.inc
// will automatically create the camera definition for you when it is
// included.

// Note that even though objects aligned using screen.inc follow the
// camera, they are still part of the scene. That means that they will be
// affected by perspective, lighting, the surroundings etc.

// For instructions of use, look in the POV-Ray manual, and for an example
// of use, see screen.pov.

#include "transforms.inc"

#ifndef(Screen_Inc_Temp)
	#declare Screen_Inc_Temp = version;
	#version 3.6;
	#ifdef(View_POV_Include_Stack)	#debug "including screen.inc\n"		#end

	#macro Update_Camera()
		#ifndef (Camera_Orthographic)	#declare Camera_Orthographic =	off;				#end
		#ifndef (Camera_Aspect_Ratio)	#declare Camera_Aspect_Ratio =	image_width/image_height;	#end
		#ifndef (Camera_Location) 	#declare Camera_Location =	<0,0,0,>;			#end
		#ifndef (Camera_Look_At)  	#declare Camera_Look_At =	+z;				#end
		#ifndef (Camera_Sky)      	#declare Camera_Sky =		+y;				#end
		#ifndef (Camera_Zoom)     	#declare Camera_Zoom =		1;				#end
		#ifndef (Camera_Wide)     	#declare Camera_Wide =		1;				#end
		#ifndef (Screen_XY)		#declare Screen_XY =		<0,0,>;				#end

		#declare CamL = Camera_Location;				// wherever you're putting it
		#declare CamD = vnormalize(Camera_Look_At - CamL);		// direction of camera view
		#declare CamR = vnormalize(vcross(Camera_Sky, CamD));		// to the right
		#declare CamU = vnormalize(vcross(CamD, CamR));			// camera up
		#if (Camera_Orthographic)
			#declare CamR = Camera_Wide * CamR;			// to the right
			#declare CamU = Camera_Wide * CamU;			// camera up
		#end

		#declare Camera_Transform = transform
		{
			matrix
			<
				CamR.x, CamR.y, CamR.z,
				CamU.x, CamU.y, CamU.z,
				CamD.x, CamD.y, CamD.z,
				CamL.x, CamL.y, CamL.z
			>
		}

		camera
		{
			#if (Camera_Orthographic)
				orthographic
			#end
			location	CamL
			direction	CamD * Camera_Zoom		// Camera_Zoom not strictly necessary with orthographic
			right		CamR
			up		CamU/Camera_Aspect_Ratio
		}
	#end

	Update_Camera()

	#macro Get_Screen_XY(Loc)
		#if (Camera_Orthographic)
			#local temp_val_1 = vlength(Camera_Look_At - Camera_Location) * tan(radians(45/2)) * 2;
			#local temp_val_2 = temp_val_1;
		#else
			#local temp_val_1 = Camera_Zoom/vlength(CamR);
			#local temp_val_2 = Camera_Zoom/vlength(CamU);
		#end
		#declare Screen_XY = vinv_transform(Loc, Camera_Transform);
		#declare Screen_XY = <1/2 + Screen_XY.x/Screen_XY.z * temp_val_1, 1/2 - Screen_XY.y/Screen_XY.z * temp_val_2 * Camera_Aspect_Ratio,> * <image_width, image_height,>;
	#end
	#macro Set_Camera_Location(Loc)
		#declare Camera_Location = Loc;
		Update_Camera()
	#end
	#macro Set_Camera_Look_At(LookAt)
		#declare Camera_Look_At = LookAt;
		Update_Camera()
	#end
	#macro Set_Camera_Aspect_Ratio(Aspect)
		#declare Camera_Aspect_Ratio = Aspect;
		Update_Camera()
	#end
	#macro Set_Camera_Aspect(Width,Height)
		#declare Camera_Aspect_Ratio = Width/Height;
		Update_Camera()
	#end
	#macro Set_Camera_Sky(Sky)
		#declare Camera_Sky = Sky;
		Update_Camera()
	#end
	#macro Set_Camera_Zoom(Zoom)
		#declare Camera_Zoom = Zoom;
		Update_Camera()
	#end
	#macro Set_Camera_Angle(Angle)
		#declare Camera_Zoom = 1/tan(radians(Angle/2));
		#declare Camera_Wide = vlength(Camera_Look_At - Camera_Location)/Camera_Zoom;
		Update_Camera()
	#end
	#macro Set_Camera(Location, LookAt, Angle, Ortho)
		#declare Camera_Angle = Angle;
		#declare Camera_Orthographic = Ortho;
		#declare Camera_Location = Location;
		#declare Camera_Look_At = LookAt;
		Set_Camera_Angle(Angle)
		Update_Camera()
	#end
	#macro Reset_Camera()
		#undef Camera_Orthographic
		#undef Camera_Location
		#undef Camera_Aspect_Ratio
		#undef Camera_Location
		#undef Camera_Look_At
		#undef Camera_Sky
		#undef Camera_Zoom
		#undef Camera_Wide
		#undef Screen_XY
		Update_Camera()
	#end

	#macro Screen_Object(Object, Position, Spacing, Confine, Scaling)
		#local Obj_Max = max_extent(Object);
		#local Obj_Min = min_extent(Object);
		#local Obj_Cen = (Obj_Max + Obj_Min)/2;
		#local Obj_Dim = (Obj_Max - Obj_Min)/2;
		#local Pos = (Position - 1/2) * 2;
		#local Pos =
		(
			+ <Pos.x/2, Pos.y/Camera_Aspect_Ratio/2, 0,>
			+ (0 - Obj_Cen - Pos * (Obj_Dim + Spacing)) * Confine
		);
		object
		{
			Object
			no_shadow		// shouldn't cast shadows in the scene
			no_reflection		// shouldn't be reflected in scene elements
			translate	Pos
			#if (Camera_Orthographic)
				translate	+z * Camera_Zoom * Scaling
			#else
				translate	+z * Camera_Zoom
				scale		Scaling
			#end
			transform {Camera_Transform}
		}
	#end
	#macro Screen_Plane(Texture, Scaling, BLCorner, TRCorner)
		box
		{
			<-0.000001,-0.000001,0,>, <+1.000001,+1.000001,0,>
			texture {Texture}
			scale		TRCorner * <1,1,0,> - BLCorner * <1,1,0,> + z
			translate	BLCorner * <+1/1,+1/1,0,> + <-1/2,-1/2,0,>
			scale		<1,1/Camera_Aspect_Ratio,1,>
			no_shadow		// shouldn't cast shadows in the scene
			no_reflection		// shouldn't be reflected in scene elements
			hollow		on	// for media/fog
			#if (Camera_Orthographic)
				translate	+z * Camera_Zoom * Scaling
			#else
				translate	+z * Camera_Zoom
				scale		Scaling
			#end
			transform {Camera_Transform}
		}
	#end

	#version Screen_Inc_Temp;
#end
