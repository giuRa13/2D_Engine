#include "SceneSerializer.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>


/*namespace YAML 
{

}*/

namespace ENGINE
{
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
    }

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap; // TagComponent
		}

		/*if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap; // TransformComponent
		}*/

		out <<YAML::EndMap; // Entity
	}
	
	void SceneSerializer::Serialize(const std::string& filepath)
	{
        YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		
		//m_Scene->m_Registry.each([&](auto entityID)
        m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;
			SerializeEntity(out, entity);
		});
        
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
	}

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {

    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
        // Not implemented
		return false;
	}
}