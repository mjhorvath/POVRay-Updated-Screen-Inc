// This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
// letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

//               Persistence of Vision Raytracer Version 3.7
//                           Screen Include File

// Created by Christoph Hormann, Chris Huff, Rune S. Johansen and Michael 
// Horvath.

// Screen.inc will enable you to place objects and textures right in front
// of the camera. One use of this is to place your signature or a logo in
// the corner of the image.

// You can use screen.inc with either a perspective or an orthographic 
// camera. Screen.inc will automatically create the camera definition for 
// you when it is included.

// Note that even though objects aligned using screen.inc follow the
// camera, they are still part of the scene. That means that they will be
// affected by perspective, lighting, the surroundings etc.

// For instructions of use, look in the POV-Ray manual, and for an example
// of use, see screen.pov.

#include "transforms.inc"

#ifndef(Screen_Inc_Temp)
#declare Screen_Inc_Temp = version;
#version 3.7;

#ifdef(View_POV_Include_Stack)
   #debug "including screen.inc\n"
#end

#macro Update_Camera()
	#ifndef (Camera_Direct_Mode)	#declare Camera_Direct_Mode =	0;				#end
	#ifndef (Camera_Orthographic)	#declare Camera_Orthographic =	off;				#end
	#ifndef (Camera_Aspect_Ratio)	#declare Camera_Aspect_Ratio =	image_width/image_height;	#end
	#ifndef (Camera_Location)	#declare Camera_Location =	<0,0,0,>;			#end
	#ifndef (Camera_Look_At) 	#declare Camera_Look_At =	+z;				#end
	#ifndef (Camera_Sky)	  	#declare Camera_Sky =		+y;				#end
	#ifndef (Camera_Angle)		#declare Camera_Angle =		60;				#end
	#ifndef (Camera_Zoom)		#declare Camera_Zoom =		1;				#end
	#ifndef (Camera_Direction)	#declare Camera_Direction =	+z;				#end
	#ifndef (Camera_Right)		#declare Camera_Right =		-z;				#end
	#ifndef (Camera_Up)		#declare Camera_Up =		+y;				#end
	#ifndef (Camera_Transform)	#declare Camera_Transform =	transform{}			#end
	#ifndef (Camera_Transform_Mtx)	#declare Camera_Transform_Mtx =	transform{}			#end
	#ifndef (Camera_Transform_All)	#declare Camera_Transform_All =	transform{}			#end

	#local CamL = Camera_Location;			// wherever you're putting it
	#if (Camera_Direct_Mode)			// Look_At or Direction?
		#local CamD = Camera_Direction;		// direction of camera view
		#local CamR = Camera_Right;		// to the right
		#local CamU = Camera_Up;		// camera up

		#declare Camera_Look_At =	CamL + CamD;
		#declare Camera_Sky =		CamU;
		#declare Camera_Angle =		atan(vlength(CamR)/vlength(CamD));
		#declare Camera_Zoom =		1/tan(Camera_Angle/2);
	#else
		#local CamD = vnormalize(Camera_Look_At - CamL);
		#local CamR = vnormalize(vcross(Camera_Sky, CamD));
		#local CamU = vnormalize(vcross(CamD, CamR))/Camera_Aspect_Ratio;
//		#local CamU = vnormalize(vcross(CamD, CamR));

		#if (Camera_Orthographic)
			#local CamW = vlength(Camera_Look_At - Camera_Location)/Camera_Zoom;
			#local CamR = CamW * CamR;
			#local CamU = CamW * CamU;
		#else
			#local CamD = CamD * Camera_Zoom;
		#end

		#declare Camera_Direction =	CamD;
		#declare Camera_Right =		CamR;
		#declare Camera_Up =		CamU;
	#end

	#declare Camera_Transform_Mtx = transform
	{
		matrix
		<
			CamR.x, CamR.y, CamR.z,
			CamU.x, CamU.y, CamU.z,
			CamD.x, CamD.y, CamD.z,
			CamL.x, CamL.y, CamL.z
		>
	}
	#declare Camera_Transform_All = transform
	{
		transform {Camera_Transform_Mtx}
		transform {Camera_Transform}
	}

	camera
	{
		#if (Camera_Orthographic)
			orthographic
		#else
			perspective
		#end
		location	CamL
		direction	CamD
		right		CamR
		up		CamU
		transform {Camera_Transform}
	}
#end

#macro Get_Screen_XY(Loc)
	#local temp_val = vinv_transform(Loc, Camera_Transform_All);
	#if (Camera_Orthographic)
		#local temp_val = <1/2 + temp_val.x, 1/2 - temp_val.y,>;
	#else
		#local temp_val = <1/2 + temp_val.x/temp_val.z, 1/2 - temp_val.y/temp_val.z,>;
	#end
	// return value
	temp_val * <image_width, image_height>
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
	#declare Camera_Angle = 2 * degrees(atan(1/Zoom));
	Update_Camera()
#end
#macro Set_Camera_Angle(Angle)
	#declare Camera_Angle = Angle;
	#declare Camera_Zoom = 1/tan(radians(Angle/2));
	Update_Camera()
#end
#macro Set_Camera_Direction(Direction)
	#declare Camera_Direction = Direction;
	Update_Camera()
#end
#macro Set_Camera_Right(Right)
	#declare Camera_Right = Right;
	Update_Camera()
#end
#macro Set_Camera_Up(Up)
	#declare Camera_Up = Up;
	Update_Camera()
#end
#macro Set_Camera_Transform(Transform)
	#declare Camera_Transform = Transform;
	Update_Camera()
#end
#macro Set_Camera_Direct_Mode(Mode)
	#declare Camera_Direct_Mode = Mode;
	Update_Camera()
#end
#macro Set_Camera_Orthographic(Ortho)
	#declare Camera_Orthographic = Ortho;
	Update_Camera()
#end
#macro Set_Camera(Location, LookAt, Angle)
	#declare Camera_Location = Location;
	#declare Camera_Look_At = LookAt;
	#declare Camera_Angle = Angle;
	#declare Camera_Direct_Mode = 0;
	Set_Camera_Angle(Angle)
	Update_Camera()
#end
#macro Set_Camera_Alt(Location, Direction, Right, Up)
	#declare Camera_Location = Location;
	#declare Camera_Direction = Direction;
	#declare Camera_Right = Right;
	#declare Camera_Up = Up; 		
	#declare Camera_Direct_Mode = 1;
	Update_Camera()
#end
#macro Reset_Camera()
	#undef Camera_Direct_Mode
	#undef Camera_Orthographic
	#undef Camera_Location
	#undef Camera_Aspect_Ratio
	#undef Camera_Location
	#undef Camera_Look_At
	#undef Camera_Sky
	#undef Camera_Zoom
	#undef CamW
	#undef Camera_Direction
	#undef Camera_Right
	#undef Camera_Up
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
			translate	+z * Scaling
		#else
			translate	+z
			scale		Scaling
		#end
		scale y * Camera_Aspect_Ratio
		transform {Camera_Transform_All}
	}
#end
#macro Screen_Plane(Texture, Scaling, BLCorner, TRCorner)
	box
	{
		<-0.000001,-0.000001,0,>, <+1.000001,+1.000001,0,>
		texture {Texture}
		scale		TRCorner * <1,1,0,> - BLCorner * <1,1,0,> + z
		translate	BLCorner * <+1/1,+1/1,0,> + <-1/2,-1/2,0,>
		no_shadow		// shouldn't cast shadows in the scene
		no_reflection		// shouldn't be reflected in scene elements
		hollow		on	// for media/fog
		#if (Camera_Orthographic)
			translate	+z * Scaling
		#else
			translate	+z
			scale		Scaling
		#end
		transform {Camera_Transform_All}
	}
#end

#version Screen_Inc_Temp;
#end
