/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#pragma once
#include "pxccapture.h"

/**
	Instance of this interface class can be created using 
	PXCScenePerception::CreatePXCSurfaceVoxelsData method. ExportSurfaceVoxels
	function fills the data buffer. It's clients responsibility to 
	explicitly release the memory by calling Release() On PXCSurfaceVoxelsData
*/
class PXCSurfaceVoxelsData : public PXCBase
{
public:

	PXC_CUID_OVERWRITE(PXC_UID('S', 'P', 'S', 'V'));

	/**
	    @brief: Returns number of Surface Voxels present in the buffer. 
		This function is expected to be used after successful
		call to ExportSurfaceVoxels()
	*/
	virtual pxcI32 PXCAPI QueryNumberOfSurfaceVoxels() = 0;

	/**
	    @brief: Returns an array to center of Surface Voxels extracted by 
		ExportSurfaceVoxels. This function is expected to be used after successful
		call to ExportSurfaceVoxels(). Valid range is [0, 3*QueryNumberOfSurfaceVoxels())
	*/
	virtual pxcF32 * PXCAPI QueryCenterOfSurfaceVoxels() = 0;

	/**
	    @brief: Sets NumberOfSurfaceVoxels to 0. However it doesn't release memory. 
	    It should be used when you reset ScenePerception using 
	    PXCScenePerception::Reset() client should Reset PXCSurfaceVoxelsData 
	    when Scene Perception is Reset to stay in Sync with the ScenePerception
	*/
	virtual pxcStatus PXCAPI Reset() = 0;

	/**
	    @brief: Returns an array of colors with length 3*QueryNumberOfSurfaceVoxels(). Three
		color channels (RGB) per voxel. This Function will return NULL, if 
		PXCSurfaceVoxelsData was created using 
		PXCScenePerception::CreatePXCSurfaceVoxelsData with bUseColor set to 0
	*/
	virtual pxcBYTE * PXCAPI QuerySurfaceVoxelsColor() = 0;
};

/**
	An instance of this interface can be created using 
	PXCScenePerception::CreatePXCBlockMeshingData method DoMeshingUpdate
	function fills all the buffer with the data. It's clients responsibility to 
	explicitly release the memory by calling Release() On PXCBlockMeshingData
*/
class PXCBlockMeshingData : public PXCBase 
{
	public:

	PXC_CUID_OVERWRITE(PXC_UID('S', 'P', 'B', 'M'));

	/**
	    @brief: Returns Number of PXCBlockMesh present inside the buffer
		returned by QueryBlockMeshes(). This function is expected to be used 
		after successful call to DoMeshingUpdate
	*/
	virtual pxcI32 PXCAPI QueryNumberOfBlockMeshes() = 0; 

	/**
	    @brief: Returns number of Vertices present in the buffer returned by 
		QueryVertices(). This function is expected to be used after successful
		call to DoMeshingUpdate
	*/
	virtual pxcI32 PXCAPI QueryNumberOfVertices() = 0; 
	
	/**
	    @brief: Returns number of Faces in the buffer returned by 
		QueryFaces(). This function is expected to be used after successful
		call to DoMeshingUpdate
	*/
	virtual pxcI32 PXCAPI QueryNumberOfFaces() = 0; 
	
	/**
	    @brief: Returns maximum number of BlockMeshes that can be returned by 
		DoMeshingUpdate. This value remains same throughout the lifetime of the
		instance
	*/
	virtual pxcI32 PXCAPI QueryMaxNumberOfBlockMeshes() = 0;
	
	/**
	    @brief: Returns maximum number of vertices that can be returned by 
		PXCBlockMeshingData. This value remains same throughout the lifetime of
		the instance
	*/
	virtual pxcI32 PXCAPI QueryMaxNumberOfVertices() = 0;
	
	/**
	    @brief: Returns maximum number of faces that can be returned by 
		PXCBlockMeshingData. This value remains same throughout the lifetime of 
		the instance
	*/
	virtual pxcI32 PXCAPI QueryMaxNumberOfFaces() = 0;
	
	/**
	    Describes each BlockMeshe present inside list returned by 
		QueryBlockMeshes()
	*/
	struct PXCBlockMesh
	{
		pxcI32 meshId;					// Unique ID to identify each PXCBlockMesh
		pxcI32 vertexStartIndex;		// Starting index of the vertex inside Vertex Buffer obtained using QueryVertices()
		pxcI32 numVertices;				// Total number of Vertices inside this PXCBlockMesh

		pxcI32 faceStartIndex;			// Starting index of the face list in a MeshFaces Buffer obtained using QueryFaces()
		pxcI32 numFaces;	 			// Number of faces forming the mesh inside this PXCBlockMesh 

		PXCPoint3DF32 min3dPoint;		// Minimum point for the axis aligned bounding box containing the mesh piece
		PXCPoint3DF32 max3dPoint;		// Maximum point for the axis aligned bounding box containing the mesh piece

		pxcF32 maxDistanceChange;		// Maximum change in the distance field due to accumulation since this block was last meshed
		pxcF32 avgDistanceChange;		// Average change in the distance field due to accumulation since this block was last meshed
	};

	/**
	    @brief: Returns an array of PXCBlockMesh objects with length same as 
		QueryNumberOfBlockMeshes()
	*/
	virtual PXCBlockMesh * PXCAPI QueryBlockMeshes() = 0;  
	
	/**
	    @brief: Returns an array of float points with length 4*QueryNumberOfVertices().
	    Each vertex consists of 4 float points: (x, y, z) coordinates in meter
		unit + a confidence value. The confidence value is in the range [0, 1] 
		indicating how confident scene perception is about the presence of 
		the vertex. 
	*/
	virtual pxcF32 *  PXCAPI QueryVertices() = 0;
	
	/**
	    @brief: Returns an array of colors with length 3*QueryNumberOfVertices(). Three
		color channels (RGB) per vertex. This Function will return NULL, if 
		PXCBlockMeshingData was created using 
		PXCScenePerception::CreatePXCBlockMeshingData with bUseColor set to 0
	*/
	virtual pxcBYTE * PXCAPI QueryVerticesColor() = 0;
	
	/**
	    @brief: Returns an array of faces forming the mesh (3 pxcI32 indices 
		per triangle) valid range is from [0, 3*QueryNumberOfFaces()]
	*/
	virtual pxcI32  * PXCAPI QueryFaces() = 0;

	/**
	    @brief: Sets NumberOfBlockMeshes, NumberOfVertices and NumberOfFaces to
		0. However it doesn't release memory. It should be used when you reset 
		ScenePerception using PXCScenePerception::Reset() client should Reset 
		PXCBlockMeshingData when Scene Perception is Reset to stay in Sync with
		the ScenePerception
	*/
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
	
		enum MeshResolution
		{
			LOW_RESOLUTION_MESH, 
			MED_RESOLUTION_MESH, 
			HIGH_RESOLUTION_MESH
		};

		typedef struct
		{
			pxcBool countOfBlockMeshesRequired;
			pxcBool blockMeshesRequired;
			pxcBool countOfVeticesRequired;
			pxcBool verticesRequired;
			pxcBool countOfFacesRequired;
			pxcBool facesRequired;
			pxcBool colorsRequired;
		}MeshingUpdateInfo;

		typedef struct  
		{
			PXCSizeI32  imageSize;
			PXCPointF32 focalLength;
			PXCPointF32 principalPoint;
		}ScenePerceptionIntrinsics;

		/**
			@brief: SetVoxelResolution sets volume resolution for the scene 
			perception The VoxelResolution is locked when 
			PXCSenseManager::Init(void) or 
			SetCaptureProfile(PXCVideoModule::DataDesc *) is called,
		    Afterwards Value for VoxelResolution remains same throughout the 
			runtime of Scene Perception module
		   
		    @param[in] voxelResolution: Resolution of the three dimensional 
			reconstruction. 
		    Possible values are:
			LOW_RESOLUTION:  For room-sized scenario (4/256m)
			MED_RESOLUTION:  For table-top-sized scenario (2/256m)
			HIGH_RESOLUTION: For object-sized scenario (1/256m)
		    Choosing HIGH_RESOLUTION in a room-size environment may degrade the
			tracking robustness and quality. Choosing LOW_RESOLUTION in an 
			object-sized scenario may result in a reconstructed model missing 
			the fine details. 
		   
		    @returns: PXC_STATUS_NO_ERROR if It succeeds, returns 
			PXC_STATUS_ITEM_UNAVAILABLE If called after making call to 
			PXCSenseManager::Init() or 
			SetCaptureProfile(PXCVideoModule::DataDesc *)
		*/
		virtual pxcStatus PXCAPI SetVoxelResolution(VoxelResolution /*voxelResolution*/) = 0;
		
		/**
			@brief: To Query Voxel resolution used by the scene 
			perception module. Please refer to SetVoxelResolution for more details.
		
			@returns: Returns current value of VoxelResolution used by the 
			Scene Perception module
		*/
		virtual VoxelResolution PXCAPI QueryVoxelResolution() const = 0;

		/**
			@brief: Allows user to Enable/Disable integration of upcoming 
			camera stream into 3D volume. If disabled the volume will not be 
			updated However Scene Perception will still keep tracking the 
			camera. This is a control parameter which can be updated before 
			passing every frame to the module.

			@param[in] enableFlag: Enable/Disable flag for integrating depth 
			data into the 3D volumetric representation

		    @returns: PXC_STATUS_NO_ERROR if It succeeds, otherwise returns 
			the error code
		*/
		virtual pxcStatus PXCAPI EnableSceneReconstruction(pxcBool /*enableFlag*/) = 0;

		/**
		    @brief: Allows user to check Whether integration of upcoming 
			camera stream into 3D volume is enabled or disabled. 
			
			@returns: True, If integrating depth data into the 3D volumetric
			representation is enabled
		*/
		virtual pxcBool   PXCAPI IsSceneReconstructionEnabled() = 0;

		/**
		    @brief: Allows user to set the initial camera pose.
		    This Function is only available before first frame is passed to the
			module. Once the first frame is passed the Initial Camera Pose is 
			locked and this function will be unavailable. If this function is 
			not used then the module uses default pose as the 
			Initial Pose for tracking for the device with no platform IMU and 
			for device with platform IMU the tracking pose will be computed 
			using gravity vector to align 3D volume with gravity when the 
			first frame is passed to the module.
		
		    @param[in] pose: Array of 12 pxcF32 that stores initial camera pose
		    user wishes to set in row-major order. Camera pose is specified in a 
			3 by 4 matrix [R | T] = [Rotation Matrix | Translation Vector]
		    where R = [ r11 r12 r13 ]
		              [ r21 r22 r23 ] 
		              [ r31 r32 r33 ]
		          T = [ tx  ty  tz  ]
		    Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
			Translation vector is in meters.
		    
		    @returns: If successful it returns PXC_STATUS_NO_ERROR,
			otherwise returns error code If Invalid Pose is passed or the 
			function is called after passing the first frame
		*/
		virtual pxcStatus PXCAPI SetInitialPose(const pxcF32 pose[12]) = 0;
		
		/**
		    @brief: Allows user to check tracking accuracy of the last frame 
			processed by the module. We expect users to call this function 
			after successful PXCSenseManager::AcquireFrame call and before 
			calling SenesManager::ReleaseFrame. If tracking accuracy is FAILED 
			the volume data and camera pose are not updated.

			@returns: TrackingAccuracy which can be HIGH, LOW, MED or FAILED.
		*/
		virtual TrackingAccuracy PXCAPI QueryTrackingAccuracy() = 0;

		/**
		    @brief: Allows users to access camera's latest pose. The 
			correctness of the pose depends on value obtained from 
			QueryTrackingAccuracy.

			@param[out] pose: Array of 12 pxcF32 to store camera pose in 
			row-major order. Camera pose is specified in a 3 by 4 matrix 
			[R | T] = [Rotation Matrix | Translation Vector]
			where R = [ r11 r12 r13 ]
					  [ r21 r22 r23 ] 
					  [ r31 r32 r33 ]
				  T = [ tx  ty  tz  ]
			Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
			Translation vector is in meters.

			@returns: PXC_STATUS_NO_ERROR, If the function succeeds.
			Otherwise error code will be returned.
		*/
		virtual pxcStatus PXCAPI GetCameraPose(pxcF32 pose[12]) = 0;
		 
		/**
		    @brief: Allows user to check whether the 3D volume was updated 
			since last call to DoMeshingUpdate(...).
			This function is useful for determining when to call 
			DoMeshingUpdate. 

			@returns: flag indicating that reconstruction was updated.
		*/
		virtual pxcBool PXCAPI IsReconstructionUpdated() = 0;

		/**
		    @brief: Allows user to access 2D Projection image of reconstructed 
			volume from a given camera pose by ray-casting. This function is 
			optimized for real time performance. It is also useful for 
			visualizing progress of the Scene Reconstruction. User should 
			explicitly call Release on PXCImage after copying the data.
		    or before making subsequent call to QueryVolumePreview.
		   
		    @param[in] pose: Array of 12 pxcF32 that stores initial camera pose
		    user wishes to set in row-major order. Camera pose is specified in a 
		    3 by 4 matrix [R | T] = [Rotation Matrix | Translation Vector]
		    where R = [ r11 r12 r13 ]
			 		  [ r21 r22 r23 ] 
			 		  [ r31 r32 r33 ]
			      T = [ tx  ty  tz  ]
		    Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
		    Translation vector is in meters.
		   
		    @returns: Instance of PXCImage whose content can be used for volume
			rendering. Returns NULL if there is an internal state error	or when
			the rendering is failed or When an Invalid Pose matrix is passed
		*/
		virtual PXCImage * PXCAPI QueryVolumePreview(const pxcF32 pose[12]) = 0;

		/**
		    @brief: Reset removes all reconstructed model (volume) information 
			and the Module will reinitialize the model when next Stream is 
			passed to the module. It also resets the camera pose to the one 
			provided. If the pose is not provided then the module will use 
			default pose if	there is no platform IMU on the device and in case 
			of device with platform IMU the pose will be computed using gravity
			vector to align 3D volume with gravity when the next frame is 
			passed to the module. 
			
			However it doesn't Reset instance of PXCBlockMeshingData created using 
			PXCScenePerception::CreatePXCBlockMeshingData. User should 
			explicitly call PXCBlockMeshingData::Reset to stay in sync with the 
		    reconstruction model inside Scene Perception
		    
			@param[in] pose: Array of 12 pxcF32 that stores initial camera pose
			user wishes to set in row-major order. Camera pose is specified in a 
			3 by 4 matrix [R | T] = [Rotation Matrix | Translation Vector]
			where R = [ r11 r12 r13 ]
			[ r21 r22 r23 ] 
			[ r31 r32 r33 ]
			T = [ tx  ty  tz  ]
			Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
			Translation vector is in meters.

		    @returns: On success returns PXC_STATUS_NO_ERROR. Otherwise returns
			error code like When an invalid pose argument is passed.
		*/
		virtual pxcStatus PXCAPI Reset(const pxcF32 pPose[12]) = 0;
		/**
		    @brief: Allows User to Reset Scene Perception with default Pose
		*/
		__inline pxcStatus PXCAPI Reset()
		{
			return Reset(0);
		}

		/**
		    @brief: Is an optional function meant for Expert users. It allows 
			users to set meshing thresholds for DoMeshingUpdate
		    The values set by this function will be used by succeeding calls to
			DoMeshingUpdate. Sets the thresholds indicating the magnitude of 
			changes occurring in any block that would be considered significant
			for re-meshing.

			@param[in] maxDistanceChangeThreshold: If the maximum change in a 
			block exceeds this value, then the block will be re-meshed. Setting
			the value to zero will retrieve all blocks
			
			@param[in] avgDistanceChange: If the average change in a block 
			exceeds this value, then the block will be re-meshed. 
			Setting the value to zero will retrieve all blocks

			@returns: PXC_STATUS_NO_ERROR, On success otherwise returns error code
		*/
		virtual pxcStatus PXCAPI SetMeshingThresholds(const pxcF32 maxDistanceChangeThreshold, const pxcF32 avgDistanceChange) = 0;
		
		/**
		    @brief: Allows Users to allocate PXCBlockMeshingData which can be 
			passed to DoMeshingUpdate. It's user's responsibility to explicitly 
			release the memory by calling Release()
		    
			@param[in] maxBlockMesh: Maximum number of Mesh Blocks client can 
			handle in one update from DoMeshingUpdate, If non-positive value is
			passed then it uses the default value. Use 
			PXCBlockMeshingData::QueryMaxNumberOfBlockMeshes to check the value
		    
			@param[in] maxFaces: Maximum number of Faces that client can handle
			in one update from DoMeshingUpdate, If non-positive value is passed 
			then it uses the default value. Use 
			PXCBlockMeshingData::QueryMaxNumberOfFaces to check the value

		    @param[in] maxVertices: Maximum number of Vertices that client can 
			handle in one update from DoMeshingUpdate. If non-positive value is
			passed then it uses the default value. Use 
			PXCBlockMeshingData::QueryMaxNumberOfVertices to check the value

		    @param[in] bUseColor: Flag Indicating whether user wants 
			ScenePerception to return Color per vertex in the mesh update. If 
			set the Color buffer will be created in PXCBlockMeshingData 
			Otherwise Color Buffer will not be created and any calls made to 
			PXCBlockMeshingData::QueryVerticesColor() will return NULL
		   
		    @returns: on success returns valid handle to the instance otherwise returns NULL
		*/
		virtual PXCBlockMeshingData* PXCAPI CreatePXCBlockMeshingData(const pxcI32 maxBlockMesh, const pxcI32 maxVertices, const pxcI32 maxFaces, const pxcBool bUseColor) = 0;
		
		__inline PXCBlockMeshingData* CreatePXCBlockMeshingData(const pxcI32 maxBlockMesh, const pxcI32 maxVertices, const pxcI32 maxFaces)
		{
			return CreatePXCBlockMeshingData(maxBlockMesh, maxFaces, maxVertices, 1);
		}
		__inline PXCBlockMeshingData* CreatePXCBlockMeshingData()
		{
			return CreatePXCBlockMeshingData(-1, -1, -1, 1);
		}
		
		/**
			@brief: Performs meshing and hole filling if requested. This 
			function can be slow if there is a lot of data to be meshed. For
			Efficiency reason we recommend running this function on a separate
			thread. This call is designed to be thread safe if called in 
			parallel with ProcessImageAsync. 

			@param[in] blockMeshingData: Instance of Pre-allocated 
			PXCBlockMeshingData. Refer 
			PXCScenePerception::CreatePXCBlockMeshingData For how to allocate 
			PXCBlockMeshingData
		
			@param[in] fillHoles: Argument to indicate whether to fill holes in
			mesh blocks. If set, it will fill missing details in each mesh blocks 
			that are visible from Scene Perception's camera current pose and 
			completely surrounded by closed surface(holes) by smooth linear 
			interpolation of adjacent mesh data.

			@param[in] meshingUpdateInfo: Argument to indicate which mesh 
			data you wish to use
			   -countOfBlockMeshesRequired: If set, on successful call 
			    this function will set number of block meshes available for 
			    meshing which can be retrieved using QueryNumberOfBlockMeshes()

			   -blockMeshesRequired: Can only be set to true If 
				countOfBlockMeshesRequired is set to true otherwise the value is 
				ignored, If set to true, on successful call to this function It 
				will update block meshes array in pBlockMeshingUpdateInfo which can
				be retrieved using QueryBlockMeshes()

			   -countOfVeticesRequired: If set on successful call this 
				function, It will set number of Vertices available for meshing 
				which can be retrieved using QueryNumberOfVertices()

			   -verticesRequired: Can only be set If 
				countOfVeticesRequired is set to true otherwise the value is ignored, 
				If set, on successful call to this function It will update vertices
				array in pBlockMeshingUpdateInfo which can be retrieved using 
				QueryVertices()

			   -countOfFacesRequired: If set on successful call this 
			    function, It will set number of Faces available for meshing which 
			    can be retrieved using QueryNumberOfFaces()

			   -facesRequired: Can only be set, If countOfFacesRequired 
			    is set to true otherwise the value is ignored, If set, on 
			    successful call to this function It will update Faces array in 
			    pBlockMeshingUpdateInfo which can be retrieved using QueryFaces()
	
				-colorsRequired: If set and PXCBlockMeshingData was created with color, On 
				 success function will fill in colors array which can be accessed using 
				 QueryVerticesColor()
			
			+NOTE: Set Meshing Threshold to (0, 0) prior to calling DoMeshingUpdate
			with hole filling enabled to fill mesh regions that are not changed 

			@returns: On success PXC_STATUS_NO_ERROR otherwise error code will 
			be returned
		*/
		virtual pxcStatus PXCAPI DoMeshingUpdate(PXCBlockMeshingData *blockMeshingData, const pxcBool bFillHoles, MeshingUpdateInfo* meshingUpdateInfo) = 0;

		__inline pxcStatus DoMeshingUpdate(PXCBlockMeshingData *blockMeshingData, const pxcBool bFillHoles)
		{
			return DoMeshingUpdate(blockMeshingData, bFillHoles, 0);
		}

		__inline pxcStatus DoMeshingUpdate(PXCBlockMeshingData *blockMeshingData)
		{
			return DoMeshingUpdate(blockMeshingData, 0);
		}
		
		/**
			@brief: Allow users to save mesh in an ASCII obj file in 
			MeshResolution::HIGH_RESOLUTION_MESH.
	
			@param[in] pFile: the path of the file to use for saving the mesh
		
			@param[in] bFillHoles: Indicates whether to fill holes in mesh 
			before saving the mesh

			@returns: On success PXC_STATUS_NO_ ERROR, Otherwise error code is 
			returned on failure
		*/
		virtual pxcStatus PXCAPI SaveMesh(const pxcCHAR *pFile, const pxcBool bFillHoles) = 0;
		__inline pxcStatus SaveMesh(const pxcCHAR *pFile)
		{ 
			return SaveMesh(pFile, 0); 
		}

		/** 
			@brief: Allows user to check whether the input Stream is suitable 
			for starting Scene Perception or not. We recommend using this 
			function when ScenePerceptin module is paused. User can pause 
			Scene Perception module using SenseManager::PauseScenePerception().
		    This is function should only be used as initializer to help user to determine 
		    When to start or reset scene Perception. 
			
			@param[in] PXCCapture::Sample: Input stream sample required by 
			Scene Perception module.

			@returns: Returns a value between 0.0 and 1.0 where: 
		            1.0 - represents ideal Scene for starting Scene Perception.
					0.0 - represents unsuitable Scene for 
						  starting Scene Perception.
		*/
		virtual pxcF32 PXCAPI CheckSceneQuality(PXCCapture::Sample *sample) = 0;

		/** 
		    @brief: Fills holes in the supplied depth image.

			@param[in] pDepthImage: Instance of depth image to be filled. 
			Pixels with depth value equal to zero will be linearly interpolated 
			with adjacent depth pixels. The image resolution should be 320X240

			@returns: On Success PXC_STATUS_NO_ERROR,
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI FillDepthImage(PXCImage *pDepthImage) = 0;

		/**
			@brief: Allows users to access normals of surface that are within
			view from the camera's current pose.

			@param[out] normals: Array of pre-allocated PXCPoint3DF32 to store 
			normal vectors. Each normal vector has three components namely x, y 
			and z. The size in pixels must be QVGA and hence the array size in 
			bytes should be: (PXCPoint3DF32's byte size) x (320 x 240) 

			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI GetNormals(PXCPoint3DF32* normals) = 0;

		/**
			@brief: Allows Users to access the surface's vertices
		    that are within view from camera's current pose.
		
			@param[out] vertices: Array of pre-allocated PXCPoint3DF32 to store
			vertices. Each element is a vector of x, y and z components. The 
			image size in pixels must be QVGA and hence the array
		    size in bytes should be: (PXCPoint3DF32's byte size) x (320 x 240)
		
			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI GetVertices(PXCPoint3DF32 *vertices) = 0;

		/**
		    @brief: Allows user to Save the current scene perception's state to
			a file and later supply the file to LoadState() to restore Scene 
			Perception to the saved state.
		
			@param[in] fileName: The path of the file to use for saving the 
		    scene perception state
		
			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI SaveCurrentState(const pxcCHAR* fileName) = 0;

		/**
		    @brief: Allows user to load the current scene perception's state 
			from the file that has been created using SaveCurrentState. 
			This function is only available before calling 
			PXCSenseManager::Init(). The PXCSenseManager::Init() would fail 
			If you use different camera than what was used when SaveCurrentState
			was called.
			
			@param[in] fileName: The path of the file to load Scene Perception 
			state from.
			
			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/

		virtual pxcStatus PXCAPI LoadState(const pxcCHAR* filename) = 0;

		/**
			@brief: Allows Users to allocate CreatePXCSurfaceVoxelsData which can be 
			passed to ExportSurfaceVoxels. It's user's responsibility to explicitly 
			release the instance by calling Release()

			@param[in] voxelCount: Maximum number of Voxels 
			client is expecting in each call to ExportSurfaceVoxels(...)
		    
			@param[in] bUseColor: Flag Indicating whether user wants 
			ScenePerception to return Color per voxel when ExportSurfaceVoxels(...) is 
			called. If set the Color buffer will be allocated in PXCSurfaceVoxelsData 
			Otherwise Color Buffer will not be created and any calls made to 
			PXCSurfaceVoxelsData::QuerySurfaceVoxelsColor() will return NULL

			@returns: on success returns valid handle to the instance otherwise returns NULL
		*/
		virtual PXCSurfaceVoxelsData* PXCAPI CreatePXCSurfaceVoxelsData(const pxcI32 voxelCount, const pxcBool bUseColor) = 0;
		/**
			@brief: Allows Users to allocate CreatePXCSurfaceVoxelsData without color,
			which can be passed to ExportSurfaceVoxels with default estimate of number of voxels. 
			It's users responsibility to explicitly release the instance by calling Release()
		*/
		__inline PXCSurfaceVoxelsData* CreatePXCSurfaceVoxelsData()
		{
			return  CreatePXCSurfaceVoxelsData(-1, 0);
		}

		/**
			@brief: Allows user to export the centers of the voxels intersected
		    by the surface scanned. Optionally allows to specify region of interest for 
			surface voxels to be exported. Voxels will be exported in parts over 
			multiple calls to this function. Client is expected to check return code to 
			determine if all voxels are exported successfully or not.
		   
		    @param[out] surfaceVoxelsData: Pre-allocated instance of PXCSurfaceVoxelsData 
			using CreatePXCSurfaceVoxelsData method. On Success the function will fill 
			in center of each surface voxel in an array which can be obtained using QueryCenterOfSurfaceVoxels
			and Number of voxels which can be retrieved using QueryNumberOfSurfaceVoxels()

			@param[in] lowerLeftFrontPoint: Optional, PXCPoint3DF32 represents lower 
			left corner of the front face of the bounding box which specifies region of interest for exporting 
			surface voxels 

			@param[in] upperRightRearPoint: Optional, PXCPoint3DF32 represents upper 
			right corner of the rear face of the bounding box which specifies region of interest for exporting 
			surface voxels 
		   
		    @returns: If Scene Perception module is able to export all the surface 
		    voxels it has acquired it will return PXC_STATUS_NO_ERROR and after that 
		    any calls made to ExportSurfaceVoxels will restart exporting all the
		    voxels again.
		    If all voxels cannot be fit into specified surfaceVoxelsData, it will 
		    return warning code PXC_STATUS_DATA_PENDING indicating that client 
		    should make additional calls to ExportSurfaceVoxels to get remaining 
		    voxels until PXC_STATUS_NO_ERROR is returned
		*/
		virtual pxcStatus PXCAPI ExportSurfaceVoxels(PXCSurfaceVoxelsData* surfaceVoxelsData, const PXCPoint3DF32 *lowerLeftFrontPoint, const PXCPoint3DF32 *upperRightRearPoint) = 0;
		
		/**
			@brief: Allows to Export Surface Voxels present in the entire volume
		*/
		__inline pxcStatus ExportSurfaceVoxels(PXCSurfaceVoxelsData* surfaceVoxelsData)
		{
			return ExportSurfaceVoxels(surfaceVoxelsData, 0, 0);
		}

		/**
			@brief: Allows user to get information regarding the planar 
			surfaces in the scene.

			@param[in] sample: Input stream sample required by Scene Perception module.

			@param[in] minPlaneArea: Minimum plane area to be detected in physical 
			dimension (m^2). This parameter refers to the physical size of the
			frontal planar surface at 1 meter to 3 meter from the camera. It controls 
			the threshold for the number of planes to be returned. Setting it to a
			smaller value makes the function to return smaller planes as well.
			E.g 0.213X0.213 m^2. The maximum acceptable value is 16.

			@param [in] maxPlaneNumber: Maximum number of planes that user wish
			to detect, It should also match Number of rows of the equation 
			array pPlaneEq

			@param[out] pPlaneEq: Pre-allocated float array for storing the 
			plane equations detected by the function. Each row contains the 
			coefficients {a,b,c,w} of the detected plane, Hence the number of 
			rows are equal to maxPlaneNumber. a, b, c are co-efficients of 
			normalized plane equation and w is in meters.
			E.g. Row 0 of pPlaneEq will contain the plane equation: ax+by+cz+w
			in the form	pPlaneEq[0][0] to pPlaneEq[0][3] = {a,b,c,w}. Similarly
			rest of the rows will provide the equations for the remaining 
			planes. Rows for which planes are not detected will have all values 0.
			
			@param[out] pPlaneIndexImg: Pre-allocated array of (320X240) to store plane 
			ids. On Success each index will have one of the following values 
				- 0: If the pixel is not part of any detected planes 
				- 1: If the pixel is part of the first plane detected
				- 2: If the pixel is part of the second plane is detected
				and so on.

			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI ExtractPlanes(PXCCapture::Sample *sample, pxcF32 minPlaneArea, pxcI32 maxPlaneNumber,
											   pxcF32 pPlaneEq[][4], pxcBYTE *pPlaneIndexImg) = 0;
		
		/**
			@brief: Allows user to set Meshing resolution for DoMeshingUpdate(...)

			@param[in] meshResolution: Mesh Resolution user wishes to set

			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI SetMeshingResolution(MeshResolution meshResolution) = 0;

		/**
		    @brief: Allows user to get Meshing resolution used by DoMeshingUpdate(...)
		
		    @returns: MeshResolution used by DoMeshingUpdate(...)
		*/
		virtual MeshResolution PXCAPI QueryMeshingResolution() = 0;

		/**
		    @brief: Allows user to get Meshing thresholds used by Scene 
			Perception.

			@param[out] maxDistanceChangeThreshold: Pre-allocated pxcF32 where 
			user wishes to retrieve max distance change threshold

			@param[out] avgDistanceChange: Pre-allocated pxcF32 where 
			user wishes retrieve average distance change threshold

			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI QueryMeshingThresholds(pxcF32 *maxDistanceChangeThreshold, pxcF32 *avgDistanceChange) = 0;

		/** 
			@brief: Allows user to set region of interest for Meshing. If Used,
			The DoMeshingUpdate(...) function will only 
			mesh these specified regions. Use ClearMeshingRegion to clear the 
			meshing region set by this function.

			@param[in] lowerLeftFrontPoint: Pre-allocated PXCPoint3DF32 Which 
			specifies lower left corner of the front face of the bounding box
		
			@param[in] upperRightRearPoint: Pre-allocated PXCPoint3DF32 which specifies 
			upper right corner of the rear face of the bounding box

			@returns: On success PXC_STATUS_NO_ERROR,			
			Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI SetMeshingRegion(const PXCPoint3DF32 *lowerLeftFrontPoint, const PXCPoint3DF32 *upperRightRearPoint) = 0;

		/**
			@brief: Allows user to clear meshing region set by SetMeshingRegion

			@returns: On success PXC_STATUS_NO_ERROR,			
				Otherwise error code will be returned on failure
		*/
		virtual pxcStatus PXCAPI ClearMeshingRegion() = 0;

		/**
			@brief: Allows user to enforce the supplied pose as the camera pose 
			.The module will track the camera from this pose when the next 
			frame is passed. This function can be called anytime after module finishes 
			processing first frame Or anytime after module successfully processes the 
			first frame post a call to Reset Scene Perception.
			
			@param[in] pose: Array of 12 pxcF32 that stores the camera pose
			user wishes to set in row-major order. Camera pose is specified in a 
			3 by 4 matrix [R | T] = [Rotation Matrix | Translation Vector]
			where R = [ r11 r12 r13 ]
			          [ r21 r22 r23 ] 
			          [ r31 r32 r33 ]
			      T = [ tx  ty  tz  ]
			Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
			Translation vector is in meters.
		
			@returns: PXC_STATUS_NO_ERROR, If the function succeeds.
			Otherwise error code will be returned.
		*/
		virtual pxcStatus PXCAPI SetCameraPose(const pxcF32 pose[12]) = 0;

		/**
			@brief: Allows user to get length of voxel side in meters

			@returns: Returns length of voxel side in meters
		*/
		virtual pxcF32 PXCAPI QueryVoxelSize() = 0;

		/**
			@brief: Allows user to get the intrinsics of internal scene perception 
			camera.	These intrinsics should be used with output images obtained from 
			the module. Such as QueryVolumePreview(...), GetVertices(...) and 
			GetNormals(..). This function should only be used after calling 
			PXCSenseManager::Init() otherwise it would return an error code.

			@param[out] spIntrinsics: Handle to Pre-allocated instance of 
			ScenePerceptionIntrinsics. On success this instance will be filled with 
			appropriate values.

			@returns: PXC_STATUS_NO_ERROR, If the function succeeds.
			Otherwise error code will be returned.
		*/
		virtual pxcStatus PXCAPI GetInternalCameraIntrinsics(ScenePerceptionIntrinsics *spIntrinsics) = 0;

		/**
			@brief: Allows user to integrate specified stream from supplied pose in to 
			the reconstructed volume.

			@param[in] sample: Input stream sample required by Scene Perception module.
			Obtained using PXCSenseManager::QueryScenePerceptionSample().

			@param[in] pose: Estimated Pose for the supplied input stream.
			Array of 12 pxcF32 that stores the camera pose
			user wishes to set in row-major order. Camera pose is specified in a 
			3 by 4 matrix [R | T] = [Rotation Matrix | Translation Vector]
			where R = [ r11 r12 r13 ]
			[ r21 r22 r23 ] 
			[ r31 r32 r33 ]
			T = [ tx  ty  tz  ]
			Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
			Translation vector is in meters.

			@returns: PXC_STATUS_NO_ERROR, If the function succeeds.
			Otherwise error code will be returned.
		*/
		virtual pxcStatus PXCAPI DoReconstruction(PXCCapture::Sample *sample, const pxcF32 pose[12]) = 0;

		/**
			@brief: Allows user to Enable/Disable re-localization feature of Scene 
			Perception's camera tracking. By default re-localization is enabled. This 
			functionality is only available after PXCSenseManager::Init() is called.

			@param[in] enableRelocalization: Flag specifying whether to enable or 
			disable re-localization
			
			@returns: PXC_STATUS_NO_ERROR, If the function succeeds.
			Otherwise error code will be returned.
		*/
		virtual pxcStatus PXCAPI EnableRelocalization(pxcBool enableRelocalization) = 0;

		/**
			@brief: Allows user to Transform plane equations obtained from
			ExtractPlanes(...) to world co-ordinate system using the provided 
			pose and returns number of planes found in supplied plane equation.

			@param [in] maxPlaneNumber: Number of rows of the equation 
			array pPlaneEq

			@param[in | out] pPlaneEq: Pre-allocated float array plane equations 
			obtained from  ExtractPlanes(...). On Success the plane equations will 
			be transformed in to world coordinate system using supplied camera pose.

			@param[in] pose: Array of 12 pxcF32 that stores camera pose
			of the capture sample that was supplied to the ExtractPlanes(...).
			stored in row-major order. Camera pose is specified in a 
			3 by 4 matrix [R | T] = [Rotation Matrix | Translation Vector]
			where R = [ r11 r12 r13 ]
			[ r21 r22 r23 ] 
			[ r31 r32 r33 ]
			T = [ tx  ty  tz  ]
			Pose Array Layout = [r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz]
			Translation vector is in meters.

			+NOTE: Use the pose obtained from GetCameraPose(...) 
			to transform plane equations.

			@returns: On Success, returns positive number indicating 
			number of planes found in pPlaneEq. Negative number indicates errors like 
			invalid argument.
		*/
		virtual pxcI32 PXCAPI TransformPlaneEquationToWorld(pxcI32 maxPlaneNumber, pxcF32 pPlaneEq[][4], const pxcF32 pose[12]) = 0;

};
