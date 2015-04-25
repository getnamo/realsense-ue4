/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#pragma once
#include "pxccapture.h"

class PXCBlockMeshingData : public PXCBase 
{
	public:
	///
	/// This class can be created using PXCScenePerception::CreatePXCBlockMeshingData method
	/// DoMeshingUpdate function fills all the buffer with the data
	/// It's clients resposibility to explicitly release the memory by calling Release() On PXCBlockMeshingData
	///
	PXC_CUID_OVERWRITE(PXC_UID('S', 'P', 'B', 'M'));

	// Returns Number of PXCBlockMesh present inside the buffer returned by QueryBlockMeshes()
	// This function is expected to be used after successful call to DoMeshingUpdate
	virtual pxcI32 PXCAPI QueryNumberOfBlockMeshes() = 0; 

	// Returns number of Vertices present in the buffer returned by QueryVertices()
	// This function is expected to be used after successful call to DoMeshingUpdate
	virtual pxcI32 PXCAPI QueryNumberOfVertices() = 0; 
	
	// Returns number of Faces in the buffer returned by QueryMeshFaces()
	// This function is expected to be used after successful call to DoMeshingUpdate
	virtual pxcI32 PXCAPI QueryNumberOfFaces() = 0; 
	
	// Returns maximum number of BlockMeshes that can be returned by DoMeshingUpdate
	// This value remains same throughout the lifetime of the instance
	virtual pxcI32 PXCAPI QueryMaxNumberOfBlockMeshes() = 0;
	
	// Returns maximum number of vertices that can be returned by PXCBlockMeshingData
	// This value remains same throughout the lifetime of the instance
	virtual pxcI32 PXCAPI QueryMaxNumberOfVertices() = 0;
	
	// Returns maximum number of faces that can be returned by PXCBlockMeshingData
	// This value remains same throughout the lifetime of the instance
	virtual pxcI32 PXCAPI QueryMaxNumberOfFaces() = 0;
	
	/// Describes each BlockMeshes present inside list returned by QueryBlockMeshes()
	struct PXCBlockMesh
	{
		pxcI32 meshId;					// Unique ID to identify each PXCMeshBlock
		pxcI32 vertexStartIndex;   // Starting index of the vertex inside Vertex Buffer obtained using QueryVertices()
		pxcI32 numVertices;	    // Total number of Vertices inside this PXCBlockMesh

		pxcI32 faceStartIndex;		// Starting index of the face list in a MeshFaces Buffer obained using QueryMeshFaces()
		pxcI32 numFaces;	 		// Number of faces forming the mesh inside this PXCBlockMesh 

		PXCPoint3DF32 min3dPoint;   // minimum point for the axis aligned bounding box containing the mesh piece
		PXCPoint3DF32 max3dPoint;   // maximum point for the axis aligned bounding box containing the mesh piece

		pxcF32 maxDistanceChange;			// maximum change in the distance field due to accumulation since this block was last meshed
		pxcF32 avgDistanceChange;			// average change in the distance field due to accumulation since this block was last meshed
	};

	/* Returns an array of PXCBlockMesh objects with length same as QueryNumberOfBlockMeshes()
	*/
	virtual PXCBlockMesh * PXCAPI QueryBlockMeshes() = 0;  
	
	/* Returns an array of float points with length 4*QueryVertices()
	 * Each vertex is consist of 4 float points: (x, y z) coordinates in meter unit + a confidence value
	*/
	virtual pxcF32 *  PXCAPI QueryVertices() = 0;
	
	/* Returns an array of colors with length 3*QueryVertices().
	 * Three color channels (BGR) per vertex.
	 * This Function will return NULL, if PXCBlockMeshingData was Created 
	 * using PXCScenePerception::CreatePXCBlockMeshingData with bUseColor set to <= 0
	*/
	virtual pxcBYTE * PXCAPI QueryVerticesColor() = 0;
	
	// Returns an array of faces forming the mesh (3 pxcI32 indices per triangle)
	// valid range is from [0, 3*QueryNumberOfFaces()]
	virtual pxcI32  * PXCAPI QueryMeshFaces() = 0;

	// Sets NumberOfBlockMeshes, NumberOfVertices and NumberOfFaces to 0
	// However it doesn't release memory. It should be used when you reset ScenePerception using PXCScenePerception::Reset()
	// Client should Reset PXCBlockMeshingData when Scene Perception is Reset to stay in Sync with the ScenePerception
	virtual pxcStatus PXCAPI Reset() = 0;
};

class PXCScenePerception : public PXCBase
{
	public:
		PXC_CUID_OVERWRITE(PXC_UID('S', 'C', 'N', 'P'));
		
		enum TrackingAccuracy 
		{
			HIGH, 
			LOW, 
			MED, 
			FAILED
		};

		enum VoxelResolution
		{ 
			LOW_RESOLUTION, 
			MED_RESOLUTION, 
			HIGH_RESOLUTION
		};
	
		/// <summary>
		/// SetVoxelResolution sets volume resolution for the scene perception
		/// internal state.
		/// The VoxelResolution is locked when PXCSenseManager::Init(void) 
		/// or SetCaptureProfile(PXCVideoModule::DataDesc *) is called
		/// Afterwards Value for VoxelResolution remains same throughout the runtime of Scene Perception module
		/// Users MUST check return value before calling other functions.
		/// </summary>
		///
		/// <paramref name="VoxelResolution">: resolution of the three dimensional reconstruction. 
		/// Possible values are:
		/// LOW_RESOLUTION use this resolution in a room-sized scenario (4/256m)
		/// MED_RESOLUTION use this resolution in a table-top-sized scenario (2/256m)
		/// HIGH_RESOLUTION use this resolution in a object-sized scenario (1/256m).
		/// Choosing HIGH_RESOLUTION in a room-size environment may degrade the tracking 
		/// robustness and quality. Choosing LOW_RESOLUTION in an object-sized scenario
		/// may result in a reconstructed model missing the fine details. 
		/// </paramref>
		///
		/// <returns>Returns PXC_STATUS_NO_ERROR if It succeeds
		/// returns PXC_STATUS_ITEM_UNAVAILABLE If called after making call to PXCSenseManager::Init(void) 
		/// or SetCaptureProfile(PXCVideoModule::DataDesc *)
		/// </returns>
		virtual pxcStatus PXCAPI SetVoxelResolution(VoxelResolution voxelResolution) = 0;
		
		/// <summary>
		/// QueryVoxelResolution returns current Voxel resolution used by the scene perception module.
		/// Please refer SetVoxelResolution for more details.
		/// </summary>
		///
		/// <returns>Returns current value of VoxelResolution used by the scene perception module
		/// </returns>
		virtual VoxelResolution PXCAPI QueryVoxelResolution() const = 0;

		/// <summary>
		/// EnableSceneReconstruction allows user to Enable/Disable integration of  
		/// upcoming camera stream into 3D volume. 
		/// If disabled the volume will not be updated However Scene Perception 
		/// will still keep tracking the camera
		/// This is a control parameter which can be updated before passing every frame to the module.
		/// Whenever you wish the make changes to it, We recommend calling it before startting Scene Perception module,
		/// Or before releasing the current frame or before calling ProcessImageAsync
		/// </summary>
		///
		/// <param name="enableFlag"> : When true, either integrating
		/// depth data into the 3D volumetric representation or using such 
		/// volumetric data for precise tracking.</param>
		///
		/// <returns>Returns: pxcStatus 
		virtual pxcStatus PXCAPI EnableSceneReconstruction(pxcBool enableFlag) = 0;

		/// <summary>
		/// IsSceneReconstructionEnabled allows user to to check whether integration of  
		/// upcoming camera stream into 3D volume is enabled or disabled. 
		/// </summary>
		///
		/// <returns="pxcBool"> : When true, integrating
		/// depth data into the 3D volumetric representation is enabled

		virtual pxcBool   PXCAPI IsSceneReconstructionEnabled() = 0;

		///	/// <summary>
		/// SetInitialPose can be used to set the camera pose to the pose provided or a default initial pose.
		/// This Function is only available before first frame is passed to the module.
		/// Once the first frame is passed the Initial Camera Pose is is locked 
		/// If this function is not used then module uses default pose as a Initial Pose for tracking
		/// </summary>
		///
		/// <param name="pose"> : array of 12 pxcF32
		/// that stores a camera pose. Camera pose is specified in a 3 by 4 matrix
		/// [R | T], where
		/// R = [ r11 r12 r13 ]
		///     [ r21 r22 r23 ] 
		///     [ r31 r32 r33 ]
		/// is a rotation matrix and
		/// T = [tx ty tz], is the translation vector.
		/// Camera pose is stored in the 12-element array in row-major order:
		/// [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz] </param>
		///
		/// <returns> Returns:
		/// PXC_STATUS_NO_ERROR: If Valid Pose is passed and the function is called before passing the first frame.
		virtual pxcStatus PXCAPI SetInitialPose(const pxcF32 pose[12]) = 0;
		
		/// <summary>
		///
		/// QueryTrackingStatus is an output function whose return value is updated after every frame processed by the library
		/// We expect Client to call this function after AcquireFrame call and before SenesManager::ReleaseFrame
		/// It is used to check accuracy of tracking in last frame that was passed to the scene perception module
		/// If tracking accuracy is FAILED the volume data and camera pose are not updated.
		/// </summary>
		///
		/// <returns>Returns:RACKING_STATUS it to HIGH, LOW, MED or FAILED.
		///</returns>
		virtual TrackingAccuracy PXCAPI QueryTrackingAccuracy() = 0;

		/// <summary>
		/// GetCameraPose is an output function that can be called after every frame processed by the library
		/// It provides access to the camera's latest pose.
		/// The correctnesses of the Pose depends on value obtained by QueryTrackingAccuracy
		/// </summary>
		///
		/// <param name="pose"> : array of pre-allocated pxcF32 to
		/// store camera pose. Camera pose is specified in a 3 by 4 matrix [R | T]
		/// R = [ r11 r12 r13 ]
		///     [ r21 r22 r23 ] 
		///     [ r31 r32 r33 ]
		/// is a rotation matrix and
		/// T = [tx ty tz], is the translation vector.
		/// Camera pose is stored the a 12-element array in row-major order:
		/// [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz] </param>
		///
		/// <returns>Returns
		/// PXC_STATUS_NO_ERROR : If the function succeeds.
		/// PXC_STATUS_DATA_UNAVAILABLE : If called before passing first frame to the module.
		/// </returns>
		virtual pxcStatus PXCAPI GetCameraPose(pxcF32 pose[12]) = 0;
		 
		/// <summary>
		/// IsReconstructionUpdated: This is an output function whose return value is updated 
		/// after every frame processed by the library.
		/// It is used to check whether reconstruction/3D Volume was updated 
		/// with the data from last frame passed to the Scene Perception module. 
		/// This function is useful for determining when to call DoMeshingUpdate.
		/// Some useful heuristic could be to call DoMeshingUpdate after 
		/// let's say 10 successful Update on Reconstruction since the last call to DoMeshingUpdate.
		/// This function will always return False if the client has explicitly disabled Reconstruction by calling 
		/// EnableSceneReconstruction with 0 as an argument. 
		/// </summary>
		///
		/// <returns>Returns pxcBool positive value indicating that reconstruction was updated
		/// otherwise not. 
		///</returns>
		virtual pxcBool PXCAPI IsReconstructionUpdated() = 0;

		/// <summary>
		/// QueryVolumePreview is an output function whose outcome is modified after processing of evry frame
		/// Render the reconstructed volume from a given camera pose by ray-casting.
		/// The result is an RGBA PXCImage. 
		/// this function is optimized for real time performance. 
		/// It is also useful for visualizing progress of the Scene Reconstruction
		/// User should explicitly call Release on PXCImage after copying the data.
		/// User should release PXCImage before making subsequent call to QueryVolumePreview
		/// </summary>
		///
		/// <param name="pose"> : pointer to a pre-allocated array of 12 pxcF32
		/// that stores a camera pose. Camera pose is specified in a 3 by 4 matrix
		/// [R | T], where
		/// R = [ r11 r12 r13 ]
		///     [ r21 r22 r23 ] 
		///     [ r31 r32 r33 ]
		/// is a rotation matrix and
		/// T = [tx ty tz], is the translation vector.
		/// Camera pose is stored in the 12-element array in row-major order:
		/// [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz] </param>
		///
		/// <returns>Returns :
		/// Pointer to PXCImage whose content can be used for volume rendering. 
		/// Pointer to PXCImage is set to NULL if there is an internal state error
		///	or when the rendering is failed or When an Invalid Pose matrix is passed
		/// </returns>
		virtual PXCImage * PXCAPI QueryVolumePreview(const pxcF32 pose[12]) = 0;

		/// <summary>
		/// Reset removes all reconstructed model (volume) information and 
		/// the Module will reinitializes the model when next Stream is passed to the module. 
		/// It also resets the camera pose to the one provided or otherwise uses default initial 
		/// pose . However it doesn't Reset instance of PXCBlockMeshingData created 
		/// using PXCScenePerception::CreatePXCBlockMeshingData
		/// Client should explicitly call PXCBlockMeshingData::Reset to stay in sync with the 
		/// reconstruction model inside Scene Perception
		/// </summary>
		///
		/// <param name="pose"> : Array of 12 pxcF32
		/// that stores a camera pose. Camera pose is specified in a 3 by 4 matrix
		/// [R | T], where
		/// R = [ r11 r12 r13 ]
		///     [ r21 r22 r23 ] 
		///     [ r31 r32 r33 ]
		/// is a rotation matrix and
		/// T = [tx ty tz], is the translation vector.
		/// Camera pose is stored in the 12-element array in row-major order:
		/// [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz] </param>
		/// <returns> Returns:
		/// SP_STATUS_SUCCESS: when the reset succeeds.
		/// returns error code if there is an internal state error, failure in resetting, 
		///when an invalid pose is argument passed.
		virtual pxcStatus PXCAPI Reset(const pxcF32 pPose[12]) = 0;
		__inline pxcStatus PXCAPI Reset()
		{
			return Reset(0);
		}
		/// <summary>
		/// SetMeshingThresholds is an optional function meant for Expert users.
		/// Library by default sets it to (0, 0)
		/// The values set by this function will be used by succeeding calls to DoMeshingUpdate
		/// SetMeshingThresholds sets the thresholds indicating the magnitude of changes occurring 
		/// in any block that would be considered significant for re-meshing. 
		/// </summary>
		///
		/// <param name="maxDistanceChangeThreshold"> : If the maximum change in a block exceeds this value, then 
		/// the block will be re-meshed. Setting the value to zero will retrieve all blocks.</paramref>
		///
		/// <param name="avgDistanceChangefAveDiffThreshold"> : If the average change in a block exceeds this value, then 
		/// the block will be re-meshed. Setting the value to zero will retrieve all blocks.</paramref>
		///
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success other wise return error code.</returns>
		virtual pxcStatus PXCAPI SetMeshingThresholds(const pxcF32 maxDistanceChangeThreshold, const pxcF32 avgDistanceChangefAveDiffThreshold) = 0;
		
		/// <summary>
		/// CreatePXCBlockMeshingData allows user to allocate memory for PXCBlockMeshingData which can be passed to DoMeshingUpdate
		/// Client Application should explicitly release the memory by calling Release()
		/// </summary>
		///
		/// <param name="maxBlockMesh"> : Maximum number of Mesh Blocks client can handle in one update
		/// from DoMeshingUpdate, If non-positive value is passed then it uses the default value. 
		/// Use PXCBlockMeshingData::QueryMaxNumberOfBlockMeshes to check the value</param>
		/// <param name="maxFaces"> : Maximum number of Faces that client can handle in one update
		/// from DoMeshingUpdate, If non-positive value is passed then it uses the default value.
		/// Use PXCBlockMeshingData::QueryMaxNumberOfFaces to check the value</param>
		/// <param name="maxVertices"> : Maximum number of Vertices that client can handle in one update
		/// from DoMeshingUpdate.  If non-positive value is passed then it uses the default value.
		/// Use PXCBlockMeshingData::QueryMaxNumberOfVertices to check the value</param>
		/// <param name="bUseColor"> : Indicated whether clinet wants ScenePerception to return Color per verted in the mesh update
		/// If the value passed is > 0 the Color buffer will be created in PXCBlockMeshingData
		/// Otherwise Color Buffer will not be created and any calls made to PXCBlockMeshingData::QueryVerticesColor()
		/// will return NULL</param>
		///
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success other wise return error code.</returns>
		virtual PXCBlockMeshingData* PXCAPI CreatePXCBlockMeshingData(const pxcI32 maxBlockMesh, const pxcI32 maxVertices, const pxcI32 maxFaces, const pxcBool bUseColor) = 0;
		__inline PXCBlockMeshingData* PXCAPI CreatePXCBlockMeshingData(const pxcI32 maxBlockMesh, const pxcI32 maxVertices, const pxcI32 maxFaces)
		{
			return CreatePXCBlockMeshingData(maxBlockMesh, maxFaces, maxVertices, 1);
		}
		__inline PXCBlockMeshingData* CreatePXCBlockMeshingData()
		{
			return CreatePXCBlockMeshingData(-1, -1, -1, 1);
		}
		
		/// <summary>
		/// DoMeshingUpdate performs meshing and hole filling if requested.
		/// This function is can be slow if there is lot of data to be meshed
		/// For Efficiency reason we recommend running this function on a separate thread.
		/// This call is designed to be thread safe if called in parallel with ProcessImageAsync. 
		/// </summary>
		///
		/// <param name="fillHoles"> : Argument to indicate whether to fill holes in mesh blocks
		/// </paramref>
		/// <param name="blockMeshingData"> : Poiter to Pre-allocated PXCBlockMeshingData
		/// Refer to PXCScenePerception::CreatePXCBlockMeshingData For how to allocate PXCBlockMeshingData
		/// </paramref>
		///
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success, error code on failure 
		/// Otherwise return error code /returns>
		virtual pxcStatus PXCAPI DoMeshingUpdate(PXCBlockMeshingData *blockMeshingData, const pxcBool bFillHoles) = 0;
		__inline pxcStatus PXCAPI DoMeshingUpdate(PXCBlockMeshingData *blockMeshingData)
		{
			return DoMeshingUpdate(blockMeshingData, 0);
		}

		/// <summary>
		/// SaveMesh saves a mesh from the volume in an ASCII obj file.
		/// </summary>
		///
		/// <paramref name="pFile"> the path of the file to use for saving the
		/// mesh</paramref>
		/// <paramref name="bFillHoles"> Indicates whether to fill holes in mesh before saving
		/// the mesh</paramref>
		///
		/// <returns>Returns PXC_STATUS_NO_ ERROR: on success, Otherwise error code is returned on failure
		/// </returns>
		virtual pxcStatus PXCAPI SaveMesh(const pxcCHAR *pFile, const pxcBool bFillHoles) = 0;
		__inline pxcStatus SaveMesh(const pxcCHAR *pFile)
		{ 
			return SaveMesh(pFile, 0); 
		}

		/// <summary>
		/// CheckSceneQuality determines whether the input Stream is suitable for starting Scene Perception or not
		/// We recommend using this function when ScenePerceptin module is in Paused 
		/// You can pause scene Perception module using SenseManager::PauseScenePerception()
		/// This is Function should only be used as initializer to help user to determine 
		/// when to start or reset scene Perception. 
		/// </summary>
		///
		/// <param name="PXCCapture::Sample"> : Input stream sample required by scene perception module.
		/// </param>
		///
		/// <returns>Returns a value between 0.0 and 1.0 where: 
		/// 1.0 - represents ideal Scene for starting Scene Perception.
		/// 0.0 - represents unsuitable Scene for starting Scene Perception.
		/// </returns>
		virtual pxcF32 PXCAPI CheckSceneQuality(PXCCapture::Sample *sample) = 0;

		/// <summary>
		/// FillDepthImage: Fills holes in the supplied depth image.
		/// </summary>
		///
		/// <paramref name="pDepthImage"> pointer to the depth image to be filled. Pixels with depth 
		///	value equal to zero will be filled. </paramref>
		///
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success, error code on failure 
		/// error code will be returned
		/// </returns>
		virtual pxcStatus PXCAPI FillDepthImage(PXCImage *pDepthImage) = 0;

		/// <summary>
		/// GetNormals provides access to the normals of surface points
		/// that are within view from the camera's current pose.
		/// This an output utility function which is updated every frame.
		/// This is useful for Plane or Surface detection. 
		/// </summary>
		///
		/// <param name="normals"> : Array of pre-allocated 
		/// PXCPoint3DF32 to store normal vectors. 
		/// Each normal vector has three components namely x, y and z. 
		/// The image size in pixels must be QVGA and hence the array 
		/// size in bytes is calculated as: 
		/// size = (PXCPoint3DF32's byte size) x (320 x 240) 
		/// </param>
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success, error code on failure 
		/// error code will be returned
		/// </returns>
		virtual pxcStatus PXCAPI GetNormals(PXCPoint3DF32* normals) = 0;

		/// <summary>
		/// 
		/// GetVertices function which provides access to the surface's vertices
		/// that are within view from camera's current pose.
		/// This an output utility function which is updated every frame.
		/// </summary>
		///
		/// <param name="vertices"> : Array of pre-allocated 
		/// PXCPoint3DF32 to store vertices. Each element is a vector of x, y and z
		/// components. The image size in pixels must be QVGA and hence the array
		/// size in bytes is calculated as: 
		/// size = (PXCPoint3DF32's byte size) x (320 x 240)
		/// </param> 
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success, error code on failure 
		/// error code will be returned
		/// </returns>
		virtual pxcStatus PXCAPI GetVertices(PXCPoint3DF32 *vertices) = 0;

		/// <summary>
		/// SaveCurrentState saves the current scene perception's state to a
		/// file. A later supply the file to LoadState() to restore Scene Perception to
		/// the saved state.
		/// </summary>
		///
		/// <paramref name="fileName"> the path of the file to use for saving the 
		/// scene perception state</paramref>
		///
		/// </param> 
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success, error code on failure 
		/// error code will be returned
		/// </returns>
		virtual pxcStatus PXCAPI SaveCurrentState(const pxcCHAR* fileName) = 0;

		/// <summary>
		/// LoadState loads the current scene perception's state from the
		/// file that was stored using SaveCurrentState. 
		/// </summary>
		///
		/// <paramref name="fileName"> the path of the file to load 
		/// scene perception state from</paramref>
		///
		/// </param> 
		/// <returns>Returns PXC_STATUS_NO_ERROR : on success, error code on failure 
		/// error code will be returned
		/// </returns>
		virtual pxcStatus PXCAPI LoadState(const pxcCHAR* filename) = 0;
};
