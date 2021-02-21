#ifndef POLY_LOD_SYSTEM_H
#define POLY_LOD_SYSTEM_H

#include <poly/Graphics/Renderable.h>

#include <vector>

namespace poly
{

class Shader;

///////////////////////////////////////////////////////////
/// \brief An lod system for a single renderable object
///
///////////////////////////////////////////////////////////
class LodSystem : public Renderable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Add an lod level with a renderable and a shader
	///
	/// The lod level distance is the far distance that the given
	/// renderable is rendered. Lod levels can be added in any order
	/// because it will automatically be sorted at the end of
	/// this function.
	///
	/// \param dist The far distance at which the given renderable is effective
	///
	///////////////////////////////////////////////////////////
	void addLevel(float dist, Renderable* renderable, Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Get the number of lod levels that have been added
	///
	/// \return The number of lod levels
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumLevels() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the lod distance given the lod level
	///
	/// \param The lod level index
	///
	/// \return The lod distance of a specific lod level
	///
	///////////////////////////////////////////////////////////
	float getDistance(Uint32 level) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the renderable of a lod level at a certain distance
	///
	/// \param dist A distance that determines which lod level to get a renderable from
	///
	/// \return A pointer to a renderable
	///
	///////////////////////////////////////////////////////////
	Renderable* getRenderable(float dist) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the renderable of a lod level at a certain distance
	///
	/// \param level The lod level index to get a renderable from
	///
	/// \return A pointer to a renderable
	///
	///////////////////////////////////////////////////////////
	Renderable* getRenderable(Uint32 level) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the shader of a lod level at a certain distance
	///
	/// \param dist A distance that determines which lod level to get a shader from
	///
	/// \return A pointer to a shader
	///
	///////////////////////////////////////////////////////////
	Shader* getShader(float dist) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the renderable of a lod level at a certain distance
	///
	/// \param level The lod level index to get a shader from
	///
	/// \return A pointer to a shader
	///
	///////////////////////////////////////////////////////////
	Shader* getShader(Uint32 level) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the local bounding box
	///
	/// \return The bounding box
	///
	///////////////////////////////////////////////////////////
	const BoundingBox& getBoundingBox() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the local bounding sphere
	///
	/// \brief The bounding sphere
	///
	///////////////////////////////////////////////////////////
	const Sphere& getBoundingSphere() const override;

private:
	struct LodLevel
	{
		Renderable* m_renderable;
		Shader* m_shader;
		float m_distance;
	};

private:
	std::vector<LodLevel> m_lodLevels;		//!< A list of lod levels
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::LodSystem
/// \ingroup Graphics
///
/// The lod system defines a set of renderables (models or billboards)
/// that can be rendered when attached to an entity in an octree.
/// The lod system is a renderable type, so it can be attached
/// to a RenderableComponent, which can be added to the octree
/// render system.
///
/// To use the lod system, add renderables using addLevel()
/// with the lod distance, a renderable, and a shader that
/// will be used to render the object.
///
/// The lod system's bounding volumes will always be equal to
/// the bounding volumes of the renderable in its first bounding
/// volume.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load a default shader
/// Shader shader;
/// shader.load("shaders/default.vert", Shader::Vertex);
/// shader.load("shaders/default.frag", Shader::Fragment);
/// shader.compile();
///
/// // Load three levels of lod
/// Model m1, m2, m3;
/// m1.load("models/lod1.dae");
/// m2.load("models/lod2.dae");
/// m3.load("models/lod3.dae");
///
/// LodSystem lod;
/// lod.addLevel(10.0f, &m1, &shader);	// 0.0 - 10.0
/// lod.addLevel(30.0f, &m2, &shader);	// 10.0 - 30.0
/// lod.addLevel(100.0f, &m3, &shader);	// 30.0 - 100.0
///
///
/// // Create a scene to render the lod system in
/// Scene scene;
///
/// // Create an octree
/// Octree octree;
/// octree.create();
/// scene.addRenderSystem(&octree);
///
/// // Create an entity with a transform and render component
/// scene.createEntity(TransformComponent(), RenderComponent(&lod));
///
/// // Create a camera to render from the perspective of
/// Camera camera;
///
///
/// // Game loop
/// while (true)
/// {
///		// Handle input and updates
///		// ...
///		
///		// Render the scene
///		//  - m1 gets rendered when the camera is <10 meters away
///		//  - m2 gets rendered when the camera is >10 and <30 meters away
///		//  - m3 gets rendered when the camera is >30 and <100 meters away
///		scene.render(camera);
///
///		// Display results
///		// ...
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////