/*******************************************************************************                                                                                                                                                                                                                          /*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.

*******************************************************************************/

/** @file PXCRotations.h
    Defines the PXCRotations interface, which allows creating and handling of rotation instances, using different representations.
	Various useful rotation algorithms are also supported.
	Available representations: Quaternions, Euler Angles, Rotations Matrices, Axis + Angle.
 */

#pragma once
#include "pxcbase.h"

/** @class PXCRotation 
	@brief A class factory that allows the creation of a Rotation instance using different input representations. 
	Available representations: Quaternions, Euler Angles, Rotations Matrices, Axis + Angle.     

	example code sample - create Rotation instance from quaternion and get Euler angles representation.

	PXCPoint4DF32 quaternion; // You should set the quaternion

	// Create PXCsession instance
	PXCSession* session = PXCSession_Create();

	// Create PXCRotation instance
	PXCRotation* rotation;
	session->CreateImpl<PXCRotation>(&rotation);

	//Set rotation from quaternion
	rotation->SetFromQuaternion(quaternion)

	// Query rotation in Euler angles representation
	PXCPoint3DF32 eulerAngles = rotation->QueryEulerAngles();
*/
class PXCRotation : public PXCBase
{
public:

	/* Constants */	
	PXC_CUID_OVERWRITE(PXC_UID('R','O','T','S'));
	
	/** 
	    @struct AngleAxis
		Rotation in Angle-Axis representation. Based on a rotation angle (RADIANS) around an axis
    */
	struct AngleAxis
	{
		PXCPoint3DF32 axis;
		pxcF32 angle;
	};

	/** 
	    @enum EulerOrder
		EulerOrder indicates the order in which to get the Euler angles.
		This order matters. (ROLL_PITCH_YAW !=  ROLL_YAW_PITCH)
		Pitch, Yaw and Roll are the rotation angles around the x, y and z axis accordingly.
    */
    enum EulerOrder
    {
        ROLL_PITCH_YAW, 
        ROLL_YAW_PITCH,
        PITCH_YAW_ROLL, 
        PITCH_ROLL_YAW,
        YAW_ROLL_PITCH,
        YAW_PITCH_ROLL
    };


	// Query representations
	/**			
		@brief get rotation in Euler angles representation.
		Euler angles are a 3D point that represents rotation in 3D. Each variable is the angle 
		of rotation around a certain axis (x/y/z). 
		The output vector will contain the Euler angles values in the given order.
		@param[in] order the order in which we get the Euler angles (PITCH_YAW_ROLL as default)
		@return 3D point containing Euler angles in radians.
	*/		
	virtual PXCPoint3DF32 PXCAPI QueryEulerAngles(EulerOrder order) = 0;
	inline PXCPoint3DF32 QueryEulerAngles()
	{ 
		return QueryEulerAngles(PITCH_YAW_ROLL);
	}

	/**			
		@brief get rotation in quaternion representation.
		Quaternion is a 4D point that represents rotation in 3D.
		@return 4D point containing a quaternion representation (w,x,y,z)
	*/	
	virtual PXCPoint4DF32 PXCAPI QueryQuaternion() const = 0;

	/**			
		@brief get rotation matrix representation.
		@param[out] rotation matrix - 3x3 float array, containing the rotation matrix
	*/	
	virtual void PXCAPI QueryRotationMatrix(pxcF32 (&rotationMatrix)[3][3]) = 0;

	/**			
		@brief get rotation in angle-axis representation.
		angle-axis represents rotation (angle in RADIANS) around an axis
		@return AngleAxis struct containing the axis and angle or rotation around this axis.
	*/	
	virtual AngleAxis PXCAPI QueryAngleAxis() = 0;


	// Query roll/pitch/yaw equivalents 
	/**			
		@brief get roll - angle of rotation around z axis. 
		Note that we always reproject the axis. Meaning this function is independent of QueryPitch and QueryYaw.
		@return roll - angle of rotation around the z axis 		
	*/		
	virtual pxcF32  PXCAPI QueryRoll() = 0;

	/**			
		@brief get pitch - angle of rotation around x axis. 
		Note that we always reproject the axis. Meaning this function is independent of QueryRoll and QueryYaw.
		@return pitch - angle of rotation around the x axis 		
	*/		
	virtual pxcF32 PXCAPI QueryPitch() = 0;

	/**			
		@brief get yaw - angle of rotation around y axis. 
		Note that we always reproject the axis. Meaning this function is independent of QueryPitch and QueryRoll.
		@return pitch - angle of rotation around the y axis 		
	*/	
	virtual pxcF32 PXCAPI QueryYaw() = 0;


	// Useful functions
	/**			
		@brief Set rotation as a concatenation of current rotation and the given Rotation.
		@param[in]  rotation - the given rotation
	*/	
	virtual void PXCAPI Rotate(const PXCRotation* rotation) = 0;

	/**			
		@brief Get rotated vector according to current rotation.
		@param[in] vec - the vector we want to rotate
		@return rotated vector according to current rotation.
	*/	
	virtual PXCPoint3DF32 PXCAPI RotateVector(const PXCPoint3DF32& vec) = 0;

	
	// Set rotation from each of the representations.
	/**			
		@brief Set rotation based on a quaternion.
		Quaternion is a 4D point that represents rotation in 3D.
		@param[in] quaternion rotation in quaternion representation.
	*/		
	virtual void PXCAPI SetFromQuaternion(const PXCPoint4DF32& quaternion) = 0;

	/**			
		@brief Set rotation based on Euler angles.
		Euler angles are a 3D point that represents rotation in 3D. Each variable is the angle 
		of rotation around a certain axis (x/y/z). 
		The order in which the Euler angles are given does not matter since they all represent
		the same rotation instance.
		@param[in] eulerAngle rotation in Euler angles representation.
	*/		
	virtual void PXCAPI SetFromEulerAngles(const PXCPoint3DF32& eulerAngles) = 0;

	/**			
		@brief Set rotation based on a 3x3 rotation matrix.
		Note that only rotation (not scale or translation) is taken into acount from the rotation matrix.
		That is, two matrices with the same rotation will yield the same Rotation instance regardless of their inequality.
		@param[in] rotationMatrix rotation in rotation matrix representation.
	*/		
	virtual void PXCAPI SetFromRotationMatrix(const pxcF32 (&rotationMatrix)[3][3]) = 0;

	/**			
		@brief Set rotation based on a rotation angle(RADIANS) around an axis.
		angle-axis represents rotation (angle in RADIANS) around an axis
		@param[in] angle rotation angle (RADIANS).
		@param[in] axis rotation around this axis. 
	*/		
	virtual void PXCAPI SetFromAngleAxis(const pxcF32 angle, const PXCPoint3DF32 &axis) = 0;	

	/**			
		@brief Set rotation from Spherical linear interpolation between two rotations.
		@param[in]  startRotation - start rotation
		@param[in]  endRotation - end rotation
		@param[in]  factor - interpolation factor
	*/	
	virtual void PXCAPI SetFromSlerp(pxcF32 factor, const PXCRotation* startRotation, const PXCRotation* endRotation) = 0;
};
