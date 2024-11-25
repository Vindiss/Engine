#include "Editor.h"


bool Module::Render::Editor::Editor::Init() 
{
	return true;
}

bool Module::Render::Editor::Editor::Start()
{
	engine = Engine::GetInstance();
	sceneManager = engine->GetSceneManager();
	inputModule = engine->GetModule<Module::Input::Inputs>();
	renderModule = engine->GetModule<Module::Render::Renderer>();
	TimeModule = engine->GetModule<Module::Time::Clock>();
	activeCamera = renderModule->GetActiveCamera();
	return true;
}

bool Module::Render::Editor::Editor::Update()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	selectedScene = sceneManager->GetMainScene();

	CheckMove();
	CheckIfMouseSelectGameObject(io);
	MenuBar(io);
	Inspector(io);
	SelectServOrClient(io);
	Chat(io);
	Projet(io, currentDirectory);
	GameObjectWindow(io);
	if (showAskNewSceneName) {
		AskNewSceneName(io);
	}

	ImGui::Render();

	return true;
}

bool Module::Render::Editor::Editor::Destruct()
{
	return true;
}


bool Module::Render::Editor::Editor::MenuBar(ImGuiIO& io)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("  ", NULL, window_flags);                           
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x * 1670 / 1920,io.DisplaySize.y * 8 / 1009));
	ImGui::SetWindowPos(ImVec2(0, 0));
	
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene")) 
			{
				showAskNewSceneName = true;
			}
			if (ImGui::BeginMenu("Open Scene")) 
			{
				OpenSceneMenu(sceneDirectory);
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save")) 
			{

			}
			if (ImGui::MenuItem("Exit")) 
			{
				Engine::GetInstance()->Stop();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Copy")) 
			{
				copiedGameObject = selectedGameObject;
			}
			if (ImGui::MenuItem("Paste")) 
			{
				Objects::GameObject* pasted_object = new Objects::GameObject(selectedScene, copiedGameObject);
				pasted_object->SetParent(selectedGameObject);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Assets")) 
		{
			if (ImGui::Button("Import New Assets"))
			{
				IGFD::FileDialogConfig config;
				config.path = ".";
				config.countSelectionMax = 0;
				const char* filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
				ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), "\xef\x81\xbb");
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choisir un fichier", filters, config);
			}
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
				if (ImGuiFileDialog::Instance()->IsOk()) {
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string assetsFolderPath = "EngineTest\\Assets\\Texture";
					std::filesystem::path destinationPath = assetsFolderPath + "/" + std::filesystem::path(filePathName).filename().string();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

					try {
						std::filesystem::copy_file(filePathName, destinationPath, std::filesystem::copy_options::overwrite_existing);
						std::cout << "Fichier importé avec succès vers : " << destinationPath << std::endl;
					}
					catch (const std::filesystem::filesystem_error& e) {
						std::cerr << "Erreur lors de l'importation du fichier : " << e.what() << std::endl;
					}
				}

				ImGuiFileDialog::Instance()->Close();
			}
			ImGui::EndMenu();
		}

		static bool openCreateCubePopup = false;

		if (ImGui::BeginMenu("GameObject")) 
		{
			if (ImGui::BeginMenu("3D Object")) 
			{
				if (ImGui::MenuItem("Cube"))
				{
					openCreateCubePopup = true;
				}
				if (ImGui::MenuItem("Pyramide"))
				{
					openCreatePyramidePopup = true;
				}
				if (ImGui::MenuItem("Plane"))
				{
					openCreatePlanePopup = true;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					openCreateSpherePopup = true;
				}
				if (ImGui::MenuItem("Empty"))
				{
					openCreateEmptyPopup = true;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}




		if (openCreateCubePopup) {
			ImGui::OpenPopup("Create Cube");
		}

		if (ImGui::BeginPopup("Create Cube")) {
			static char gameObjectName[64] = "Cube";
			static float position[3] = { 0.f, 0.f, 5.f };
			static float rotation[3] = { 0.f, 0.f, 0.f };

			ImGui::InputText("Name", gameObjectName, sizeof(gameObjectName));
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("Rotation", rotation);

			if (strlen(gameObjectName) == 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
				ImGui::SetItemDefaultFocus();
			}
			
			if (ImGui::Button("Create") && strlen(gameObjectName) > 0) {
				Objects::GameObject* cube = new Objects::GameObject(selectedScene);
				cube->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Cube);

				std::copy(std::begin(position), std::end(position), std::begin(position));
				cube->GetTransformReference()->position.x = position[0];
				cube->GetTransformReference()->position.y = position[1];
				cube->GetTransformReference()->position.z = position[2];

				std::copy(std::begin(rotation), std::end(rotation), std::begin(rotation));
				cube->GetTransformReference()->rotation.x = rotation[0];
				cube->GetTransformReference()->rotation.y = rotation[1];
				cube->GetTransformReference()->rotation.z = rotation[2];

				cube->SetName(gameObjectName);

				openCreateCubePopup = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Annuler")){
				openCreateCubePopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (openCreatePlanePopup) {
			ImGui::OpenPopup("Create Plane");
		}

		if (ImGui::BeginPopup("Create Plane")) {
			static char gameObjectName[64] = "Plane";
			static float position[3] = { 0.f, 0.f, 5.f };
			static float rotation[3] = { 0.f, 0.f, 0.f };

			ImGui::InputText("Name", gameObjectName, sizeof(gameObjectName));
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("Rotation", rotation);

			if (strlen(gameObjectName) == 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
				ImGui::SetItemDefaultFocus();
			}

			if (ImGui::Button("Create") && strlen(gameObjectName) > 0) {
				Objects::GameObject* plane = new Objects::GameObject(selectedScene);
				plane->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Plane);

				std::copy(std::begin(position), std::end(position), std::begin(position));
				plane->GetTransformReference()->position.x = position[0];
				plane->GetTransformReference()->position.y = position[1];
				plane->GetTransformReference()->position.z = position[2];

				std::copy(std::begin(rotation), std::end(rotation), std::begin(rotation));
				plane->GetTransformReference()->rotation.x = rotation[0];
				plane->GetTransformReference()->rotation.y = rotation[1];
				plane->GetTransformReference()->rotation.z = rotation[2];

				plane->SetName(gameObjectName);

				openCreatePlanePopup = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Annuler")) {
				openCreatePlanePopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (openCreatePyramidePopup) {
			ImGui::OpenPopup("Create Pyramide");
		}

		if (ImGui::BeginPopup("Create Pyramide")) {
			static char gameObjectName[64] = "Pyramide";
			static float position[3] = { 0.f, 0.f, 5.f };
			static float rotation[3] = { 0.f, 0.f, 0.f }; 

			ImGui::InputText("Name", gameObjectName, sizeof(gameObjectName));
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("Rotation", rotation);

			if (strlen(gameObjectName) == 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
				ImGui::SetItemDefaultFocus();
			}

			if (ImGui::Button("Create") && strlen(gameObjectName) > 0) {
				Objects::GameObject* pyramide = new Objects::GameObject(selectedScene);
				pyramide->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Pyramide);

				std::copy(std::begin(position), std::end(position), std::begin(position));
				pyramide->GetTransformReference()->position.x = position[0];
				pyramide->GetTransformReference()->position.y = position[1];
				pyramide->GetTransformReference()->position.z = position[2];

				std::copy(std::begin(rotation), std::end(rotation), std::begin(rotation));
				pyramide->GetTransformReference()->rotation.x = rotation[0];
				pyramide->GetTransformReference()->rotation.y = rotation[1];
				pyramide->GetTransformReference()->rotation.z = rotation[2];

				pyramide->SetName(gameObjectName);

				openCreatePyramidePopup = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Annuler")) {
				openCreatePyramidePopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (openCreateSpherePopup) {
			ImGui::OpenPopup("Create Sphere");
		}

		if (ImGui::BeginPopup("Create Sphere")) {
			static char gameObjectName[64] = "Sphere";
			static float position[3] = { 0.f, 0.f, 5.f };
			static float rotation[3] = { 0.f, 0.f, 0.f };

			ImGui::InputText("Name", gameObjectName, sizeof(gameObjectName));
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("Rotation", rotation);

			if (strlen(gameObjectName) == 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
				ImGui::SetItemDefaultFocus();
			}

			if (ImGui::Button("Create") && strlen(gameObjectName) > 0) {
				Objects::GameObject* sphere = new Objects::GameObject(selectedScene);
				sphere->MakeMesh("EngineTest\\Assets\\Mesh\\sphere.obj");

				std::copy(std::begin(position), std::end(position), std::begin(position));
				sphere->GetTransformReference()->position.x = position[0];
				sphere->GetTransformReference()->position.y = position[1];
				sphere->GetTransformReference()->position.z = position[2];

				std::copy(std::begin(rotation), std::end(rotation), std::begin(rotation));
				sphere->GetTransformReference()->rotation.x = rotation[0];
				sphere->GetTransformReference()->rotation.y = rotation[1];
				sphere->GetTransformReference()->rotation.z = rotation[2];

				sphere->SetName(gameObjectName);

				openCreateSpherePopup = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Annuler")) {
				openCreateSpherePopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (openCreateEmptyPopup) {
			ImGui::OpenPopup("Create Empty");
		}

		if (ImGui::BeginPopup("Create Empty")) {
			static char gameObjectName[64] = "Empty";
			static float position[3] = { 0.f, 0.f, 5.f };
			static float rotation[3] = { 0.f, 0.f, 0.f };

			ImGui::InputText("Name", gameObjectName, sizeof(gameObjectName));
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("Rotation", rotation);

			if (strlen(gameObjectName) == 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
				ImGui::SetItemDefaultFocus();
			}

			if (ImGui::Button("Create") && strlen(gameObjectName) > 0) {
				Objects::GameObject* empty = new Objects::GameObject(selectedScene);
				empty->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Empty);

				std::copy(std::begin(position), std::end(position), std::begin(position));
				empty->GetTransformReference()->position.x = position[0];
				empty->GetTransformReference()->position.y = position[1];
				empty->GetTransformReference()->position.z = position[2];

				std::copy(std::begin(rotation), std::end(rotation), std::begin(rotation));
				empty->GetTransformReference()->rotation.x = rotation[0];
				empty->GetTransformReference()->rotation.y = rotation[1];
				empty->GetTransformReference()->rotation.z = rotation[2];

				empty->SetName(gameObjectName);

				openCreateEmptyPopup = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Annuler")) {
				openCreateSpherePopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginMenu("Component")) 
		{
		
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) 
		{
		
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) 
		{
			if (ImGui::MenuItem("About")) 
			{
				OpenUrl("https://github.com/GamingCampus-AdrienBourgois/engine-mendeym");
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(std::to_string(io.Framerate).c_str()))
		{

			ImGui::EndMenu();
		}

	}

	ImGui::EndMenuBar();

	ImGui::End();

	return true;
}

void Module::Render::Editor::Editor::OpenUrl(const char* url)
{
#ifdef _WIN32
	// Utilisation de la commande shell start pour ouvrir l'URL sur Windows
	std::system(("start " + std::string(url)).c_str());
#elif defined(__unix__)
	// Utilisation de xdg-open pour ouvrir l'URL sur les systèmes Unix
	std::system(("xdg-open " + std::string(url)).c_str());
#else
	// Gestion des autres systèmes d'exploitation
	// Note : cette fonctionnalité peut ne pas être prise en charge sur tous les systèmes d'exploitation
#endif
}

bool Module::Render::Editor::Editor::Inspector(ImGuiIO& io)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Inspector", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x * 250 / 1920, io.DisplaySize.y * 900 / 1009));
	ImGui::SetWindowPos(ImVec2(io.DisplaySize.x * 1670 / 1920, 0));
		
	if (selectedGameObject) {
			
		static char gameObjectName[64] = "";
		strncpy_s(gameObjectName, sizeof(gameObjectName), selectedGameObject->GetName().c_str(), sizeof(gameObjectName) - 1);
		gameObjectName[sizeof(gameObjectName) - 1] = '\0';
		ImGui::InputText("Name", gameObjectName, sizeof(gameObjectName));
		selectedGameObject->SetName(gameObjectName);

		for (size_t i = 0; i < 4; i++)
		{
			ImGui::Spacing();
		}

		// Position
		ImGui::Text("Position:");
		ImGui::Separator();
		ImGui::Text("X:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##PosX_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->position.x -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##PositionX", &(selectedGameObject->GetTransformReference()->position.x));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##PosX_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->position.x += 0.1f;

		ImGui::Text("Y:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##PosY_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->position.y -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##PositionY", &(selectedGameObject->GetTransformReference()->position.y));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##PosY_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->position.y += 0.1f;

		ImGui::Text("Z:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##PosZ_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->position.z -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##PositionZ", &(selectedGameObject->GetTransformReference()->position.z));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##PosZ_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->position.z += 0.1f;

		// Rotation
		ImGui::Text("Rotation:");
		ImGui::Separator();
		ImGui::Text("X:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##RotationX_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->rotation.x -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##RotationX", &(selectedGameObject->GetTransformReference()->rotation.x));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##RotationX_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->rotation.x += 0.1f;

		ImGui::Text("Y:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##RotationY_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->rotation.y -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##RotationY", &(selectedGameObject->GetTransformReference()->rotation.y));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##RotationY_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->rotation.y += 0.1f;

		ImGui::Text("Z:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##RotationZ_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->rotation.z -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##RotationZ", &(selectedGameObject->GetTransformReference()->rotation.z));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##RotationZ_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->rotation.z += 0.1f;

		// Échelle
		ImGui::Text("Scale:");
		ImGui::Separator();
		ImGui::Text("X:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##ScaleX_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->scale.x -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleX", &(selectedGameObject->GetTransformReference()->scale.x));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##ScaleX_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->scale.x += 0.1f;

		ImGui::Text("Y:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##ScaleY_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->scale.y -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleY", &(selectedGameObject->GetTransformReference()->scale.y));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##ScaleY_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->scale.y += 0.1f;

		ImGui::Text("Z:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##ScaleZ_Decrease", ImGuiDir_Left))
			selectedGameObject->GetTransformReference()->scale.z -= 0.1f;
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleZ", &(selectedGameObject->GetTransformReference()->scale.z));
		ImGui::SameLine();
		if (ImGui::ArrowButton("##ScaleZ_Increase", ImGuiDir_Right))
			selectedGameObject->GetTransformReference()->scale.z += 0.1f;


		if (selectedGameObject->GetComponent<Objects::Component::GraphicComponent>() != nullptr) {
			for (size_t i = 0; i < 4; i++)
			{
				ImGui::Spacing();
			}


			ImGui::Text("Mesh : ");
			ImGui::Separator();
			if (ImGui::Button(selectedGameObject->GetComponent<Objects::Component::GraphicComponent>()->GetMesh()->GetName().c_str(), ImVec2(io.DisplaySize.x * 200 / 1920, 20))) {
				selectNewMeshPopup = true;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("##NewMesh", true)) {
				selectNewMeshPopup = true;
			}

			for (size_t i = 0; i < 4; i++)
			{
				ImGui::Spacing();
			}

			bool modified_choice_texture_color = false;
			const float* gameObjectColor = selectedGameObject->GetComponent<Objects::Component::GraphicComponent>()->GetColor();
			if (gameObjectColor[3] == 0 && choice_texture_color == 1) {
				choice_texture_color = 0;
			}
			else if(gameObjectColor[3] != 0 && choice_texture_color == 0){
				choice_texture_color = 1;
				vec_color = ImVec4(gameObjectColor[0], gameObjectColor[1], gameObjectColor[2], 1);
			}

			if (choice_texture_color == 0) {
				ImGui::Text("Texture");
			}
			else {
				ImGui::TextDisabled("Texture");
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(io.DisplaySize.x * 125 / 1920);
			if (ImGui::SliderInt("##ChoiceTextureColor", &choice_texture_color, 0, 1, "")) {
				modified_choice_texture_color = true;
			}
			ImGui::SameLine();
			if (choice_texture_color == 1) {
				ImGui::Text("Color");
			}
			else {
				ImGui::TextDisabled("Color");
			}
			if (choice_texture_color == 0) {

				std::filesystem::path pathObj(selectedGameObject->GetTexturePath());
				if (ImGui::Button(pathObj.filename().string().c_str(), ImVec2(io.DisplaySize.x * 200 / 1920, 20))) {
					selectNewTexturePopup = true;
				}
				ImGui::SameLine();
				if (ImGui::RadioButton("##NewTexture", true)) {
					selectNewTexturePopup = true;
				}
			}
			else {
				if (ImGui::ColorEdit4("Color", (float*)&vec_color)) {
					selectedGameObject->SetColor(vec_color.x, vec_color.y, vec_color.z, vec_color.w);
				}
			}
			if (modified_choice_texture_color) {
				if (choice_texture_color == 0) {
					selectedGameObject->SetColor(vec_color.x, vec_color.y, vec_color.z, 0);
				}
				else {
					vec_color = ImVec4(gameObjectColor[0], gameObjectColor[1], gameObjectColor[2], 1);
					selectedGameObject->SetColor(vec_color.x, vec_color.y, vec_color.z, 1);
				}
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			ImGui::Spacing();
		}

		
		// Script gameobject
		ImGui::InputTextWithHint("##search", "Recherche", searchBuffer, sizeof(searchBuffer));
		ImGui::SameLine();
		// Bouton pour créer un nouveau script
		if (ImGui::Button("+ Script")) {
			p_open = true;
		}

		try {
			if (fs::exists(directory) && fs::is_directory(directory)) {
				for (const auto& entry : fs::directory_iterator(directory)) {
					if (entry.is_regular_file()) {
						std::string filename = entry.path().filename().string();
						if (entry.path().extension() == ".h" && filename.find(searchBuffer) != std::string::npos) {
							if (std::find(scripts.begin(), scripts.end(), filename) == scripts.end()) {
								scripts.push_back(filename);
							}
						}
					}
				}
			}
			else {
				std::cerr << "Le répertoire spécifié n'existe pas." << std::endl;
			}
		}
		catch (const fs::filesystem_error& e) {
			std::cerr << "Erreur de système de fichiers: " << e.what() << std::endl;
		}

		for (auto& script : scripts) {
			if (strstr(script.c_str(), searchBuffer)) {
				if (scriptCheckState.find(script) == scriptCheckState.end()) {
					scriptCheckState[script] = false;
				}

				ImGui::PushID(script.c_str());
				if (ImGui::Checkbox("", &scriptCheckState[script])) {
					// action to add
				}
				ImGui::SameLine();

				// Utilisation de Selectable pour rendre le texte cliquable
				if (ImGui::Selectable(script.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
					if (ImGui::IsMouseDoubleClicked(0)) {
						// Si double cliqué, ouvrez le fichier avec Visual Studio
						std::string command = "start devenv " + directory + script;
						system(command.c_str());
					}
				}
				ImGui::PopID();
			}
		}
	}


	if (p_open) 
	{
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 200 / 1009));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 1700 / 1920, io.DisplaySize.y * 400 / 1009));
		ImGui::OpenPopup("Nouveau Script");
	}
	if (ImGui::BeginPopupModal("Nouveau Script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Nom du Script", scriptNameBuffer, sizeof(scriptNameBuffer));
		if (ImGui::Button("Ajouter")) 
		{
			if (strlen(scriptNameBuffer) > 0) 
			{
				std::string scriptName(scriptNameBuffer);
				if (scriptName.size() < 2 || scriptName.substr(scriptName.size() - 2) != ".h") {
					scriptName += ".h";
				}
				p_open = false;
				scripts.push_back(std::string(scriptName));
				ImGui::CloseCurrentPopup();
				
				std::string fullPath = directory + scriptName;
				if (!fs::exists(directory)) 
				{
					fs::create_directory(directory);
				}

				if (fs::exists(fullPath)) {
					ImGui::OpenPopup("Erreur");
				}
				else {
					std::ofstream file(fullPath);
					if (file.is_open())
					{
						scriptName.pop_back();
						scriptName.pop_back();
						file << "#pragma once" << std::endl;
						file << "#include \"Scripts/Script.h\"\n\n\n" << std::endl;
						file << "class " << scriptName << " : public Scripts::Script" << std::endl;
						file << "{" << std::endl;
						file << "public:" << std::endl;
						file << "\tbool Init() override " << std::endl;
						file << "\t{" << std::endl;
						file << "\t\tSetScriptName(\"" << scriptName << "\");" << std::endl;
						file << "\t}\n" << std::endl;
						file << "\tbool Start() override " << std::endl;
						file << "\t{" << std::endl;
						file << "\t\treturn true;" << std::endl;
						file << "\t}\n" << std::endl;
						file << "\tbool Update() override " << std::endl;
						file << "\t{" << std::endl;
						file << "\t\treturn true;" << std::endl;
						file << "\t}\n" << std::endl;
						file << "\tbool Stop() override " << std::endl;
						file << "\t{" << std::endl;
						file << "\t\treturn true;" << std::endl;
						file << "\t}\n" << std::endl;
						file << "\tbool Destruct() override " << std::endl;
						file << "\t{" << std::endl;
						file << "\t\treturn true;" << std::endl;
						file << "\t}\n" << std::endl;
						file << "private:\n" << std::endl;
						file << "};" << std::endl;
						file.close();
					}
				}
			}
		}
		if (ImGui::BeginPopupModal("Erreur", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::Text("Un fichier avec ce nom existe déjà.");
			if (ImGui::Button("OK")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Annuler")) 
		{
			p_open = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
		
	}

	// New Mesh Popup
	if (selectNewMeshPopup) {
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 200 / 1009));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 1700 / 1920, io.DisplaySize.y * 400 / 1009));
		ImGui::OpenPopup("Select New Mesh");
	}

	if (ImGui::BeginPopup("Select New Mesh", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::BeginChild("ScrollingRegion", ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 160 / 1009));
		if (meshCurrentDirectory == "EngineTest\\Assets\\Mesh")
		{
			if (ImGui::Selectable("Cube")) {
				newMeshSelected = "Cube";
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				newMeshSelected = "Cube";
				selectedGameObject->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Cube);
				selectNewMeshPopup = false;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Selectable("Plane")) {
				newMeshSelected = "Plane";
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				newMeshSelected = "Plane";
				selectedGameObject->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Plane);
				selectNewMeshPopup = false;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Selectable("Pyramide")) {
				newMeshSelected = "Pyramide";
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				newMeshSelected = "Pyramide";
				selectedGameObject->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Pyramide);
				selectNewMeshPopup = false;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Selectable("Empty")) {
				newMeshSelected = "Empty";
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				newMeshSelected = "Empty";
				selectedGameObject->MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Empty);
				selectNewMeshPopup = false;
				ImGui::CloseCurrentPopup();
			}
		}
		else if (!meshDirectoryStack.empty() && ImGui::Button("Retour")) {
			meshCurrentDirectory = meshDirectoryStack.top();
			meshDirectoryStack.pop();

		}
		for (const auto& entry : fs::directory_iterator(meshCurrentDirectory)) {
			if (entry.is_directory()) {
				if (ImGui::Selectable((entry.path().filename().string() + " (dossier)").c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
					if (ImGui::IsMouseDoubleClicked(0)) {
						meshDirectoryStack.push(meshCurrentDirectory);
						meshCurrentDirectory = entry.path().string();
					}
				}
			}
			else if (entry.is_regular_file() && entry.path().extension().string() == ".obj") {
				if (ImGui::Selectable((entry.path().filename().string() + " (fichier)").c_str())) {
					newMeshSelected = entry.path().string();
				}
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					newMeshSelected = entry.path().string();
					selectedGameObject->MakeMesh(newMeshSelected);
					selectNewMeshPopup = false;
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();

		if (ImGui::Button("Annuler")) {
			selectNewMeshPopup = false;
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}


	//New Texture Popup
	if (selectNewTexturePopup) {
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 200 / 1009));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 1700 / 1920, io.DisplaySize.y * 400 / 1009));
		ImGui::OpenPopup("Select New Texture");
	}

	if (ImGui::BeginPopup("Select New Texture", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::BeginChild("ScrollingRegion", ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 160 / 1009));
		
		if (!textureDirectoryStack.empty() && ImGui::Button("Retour")) {
			textureCurrentDirectory = textureDirectoryStack.top();
			textureDirectoryStack.pop();

		}
		for (const auto& entry : fs::directory_iterator(textureCurrentDirectory)) {
			if (entry.is_directory()) {
				if (ImGui::Selectable((entry.path().filename().string() + " (dossier)").c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
					if (ImGui::IsMouseDoubleClicked(0)) {
						textureDirectoryStack.push(textureCurrentDirectory);
						textureCurrentDirectory = entry.path().string();
					}
				}
			}
			else if (entry.is_regular_file()) {
				if (ImGui::Selectable((entry.path().filename().string() + " (fichier)").c_str())) {
					newTextureSelected = entry.path().string();
				}
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					newTextureSelected = entry.path().string();
					selectedGameObject->MakeTexture(newTextureSelected);
					selectNewTexturePopup = false;
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();

		if (ImGui::Button("Annuler")) {
			selectNewTexturePopup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
	return true;
}

bool Module::Render::Editor::Editor::Projet(ImGuiIO& io, const std::string& directory)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Projet", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x * 800 / 1920, io.DisplaySize.y * 201 / 1009));
	ImGui::SetWindowPos(ImVec2(0, io.DisplaySize.y * 825 / 1009));


	ImGui::Columns(1);
	ImGui::Separator();


	if (!directoryStack.empty() && ImGui::Button("Retour")) {
		currentDirectory = directoryStack.top();
		directoryStack.pop();
	}

	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_directory()) {
			if (ImGui::Selectable((entry.path().filename().string() + " (dossier)").c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(0)) {
					directoryStack.push(currentDirectory);
					currentDirectory = entry.path().string();
				}
			}
		}
		else if (entry.is_regular_file()) {
			if (ImGui::Selectable((entry.path().filename().string() + " (fichier)").c_str())) {
				selectedFilePath = entry.path().string();
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
				// Si le clic droit est utilisé sur ce fichier, ouvrez le menu contextuel
				ImGui::OpenPopup("MenuContextuel");
				selectedFilePath = entry.path().string();
			}
		}
	}

	// Menu contextuel pour la suppression
	if (ImGui::BeginPopup("MenuContextuel")) {
		if (ImGui::Selectable("Delete")) {
			// Supprimer le fichier sélectionné
			fs::remove(selectedFilePath);
			selectedFilePath.clear();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
	return true;
}

bool Module::Render::Editor::Editor::GameObjectWindow(ImGuiIO& io)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin(selectedScene->GetName().c_str(), NULL, window_flags);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 794 / 1009));
	ImGui::SetWindowPos(ImVec2(0, io.DisplaySize.y * 31 / 1009));
	for (auto gameObject : *selectedScene->GetGameObjects()) 
	{
		if (!gameObject->HaveParent()) 
		{
			GameObjectList(gameObject);
		}
	}
	if (ImGui::BeginPopup("GameObjectMenu")) {
		if (ImGui::Selectable("Copy")) {
			copiedGameObject = actualGameObjectOnMenu;
			actualGameObjectOnMenu == nullptr;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Paste")) {
			Objects::GameObject* pasted_object = new Objects::GameObject(selectedScene, copiedGameObject);
			pasted_object->SetParent(actualGameObjectOnMenu);
			actualGameObjectOnMenu == nullptr;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Delete")) {
			for (Objects::GameObject* go : *actualGameObjectOnMenu->GetChild()) {
				selectedScene->DeleteGameObject(go);
			}
			selectedScene->DeleteGameObject(actualGameObjectOnMenu);
			delete actualGameObjectOnMenu;
			if (selectedGameObject == actualGameObjectOnMenu) {
				selectedGameObject = nullptr;
			}
			actualGameObjectOnMenu = nullptr;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
	return true;
}

bool Module::Render::Editor::Editor::GameObjectList(Objects::GameObject* gameObject)
{
	if (gameObject->HaveChilds()) {
		if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
			if (ImGui::IsMouseClicked(0)) {
				selectedGameObject = gameObject;
			}
			if (ImGui::IsMouseClicked(1)) {
				actualGameObjectOnMenu = gameObject;
				ImGui::OpenPopup("GameObjectMenu");
			}
			for (auto child : *gameObject->GetChild()) {
				GameObjectList(child);
			}
			ImGui::TreePop();
		}
	}
	else {
		if (ImGui::Selectable(gameObject->GetName().c_str(), gameObject == selectedGameObject)) {
			selectedGameObject = gameObject;
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			actualGameObjectOnMenu = gameObject;
			ImGui::OpenPopup("GameObjectMenu");
		}
	}
	return true;
}

bool Module::Render::Editor::Editor::AskNewSceneName(ImGuiIO& io)
{
	ImVec2 centerScreen = ImVec2((io.DisplaySize.x - 200) * 0.5f, (io.DisplaySize.y - 100) * 0.5f); 
	ImGui::SetNextWindowPos(centerScreen, ImGuiCond_Always, ImVec2(0.5f, 0.5f)); // Positionner la fenêtre au centre de l'écran
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 200 / 1920, io.DisplaySize.y * 100 / 1009));
	ImGui::Begin("New Scene", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Nom de la nouvelle scene :");
	ImGui::InputText("##NewSceneName", newSceneName, sizeof(newSceneName));
	if (ImGui::Button("Creer"))
	{
		std::string sceneName(newSceneName);
		CreateNewScene(sceneName);
		newSceneName[0] = '\0';
		showAskNewSceneName = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Annuler"))
	{
		showAskNewSceneName = false;
	}

	ImGui::End();

	return true;
}

bool Module::Render::Editor::Editor::CreateNewScene(std::string _new_scene_name) 
{
	std::string filename = "EngineTest\\Assets\\Scene\\" + _new_scene_name + ".h";

	std::ofstream file(filename);
	if (file.is_open()) {
		file << "#pragma once" << std::endl;
		file << "#include \"Core/Interface/IScene.h\"\n\n\n" << std::endl;
		file << "class " << _new_scene_name << " : public Core::Interface::IScene" << std::endl;
		file << "{" << std::endl;
		file << "public:" << std::endl;
		file << "\t" << _new_scene_name << "() = default;" << std::endl;
		file << "\t~" << _new_scene_name << "() = default;\n" << std::endl;
		file << "\tbool Init() override " << std::endl;
		file << "\t{" << std::endl;
		file << "\t\tSetName(\"" << _new_scene_name << "\");" << std::endl;
		file << "\t\tSetFilePath(\"" << filename << "\");" << std::endl;
		file << "\t\treturn true;" << std::endl;
		file << "\t}\n" << std::endl;
		file << "\tbool Start() override " << std::endl;
		file << "\t{" << std::endl;
		file << "\t\treturn true;" << std::endl;
		file << "\t}\n" << std::endl;
		file << "\tbool Update() override " << std::endl;
		file << "\t{" << std::endl;
		file << "\t\treturn true;" << std::endl;
		file << "\t}\n" << std::endl;
		file << "\tbool Stop() override " << std::endl;
		file << "\t{" << std::endl;
		file << "\t\treturn true;" << std::endl;
		file << "\t}\n" << std::endl;
		file << "\tbool Destruct() override " << std::endl;
		file << "\t{" << std::endl;
		file << "\t\treturn true;" << std::endl;
		file << "\t}\n" << std::endl;
		file << "private:\n" << std::endl;
		file << "};" << std::endl;
		file.close();

	}
	else {
		return false;
	}
	std::string filename2 = "EngineTest\\EngineTest.cpp";

	std::ifstream cppread(filename2);
	std::vector<std::string> lines;
	if (cppread.is_open()) {
		std::string line;
		while (std::getline(cppread, line)) {
			if (line.find("SetScene") != std::string::npos) {
				std::string new_line = "\tengine->AddSceneType<" + _new_scene_name + ">();\n";
				lines.push_back(new_line);
			}
			lines.push_back(line);
		}
		cppread.close();
	}
	else {
		return false;
	}

	std::ofstream cppwrite(filename2);
	if (cppwrite.is_open()) {
		cppwrite << "#include \"Assets/Scene/" << _new_scene_name << ".h\"" << std::endl;
		for (std::string line : lines) {
			cppwrite << line << std::endl;
		}
		cppwrite.close();
	}
	else {
		return false;
	}

	Core::Interface::IScene* scene = static_cast<Core::Interface::IScene *>(new NewScene());
	scene->SetName(_new_scene_name);
	scene->SetFilePath(filename);
	sceneManager->SetNewScene(scene);

	return true;
}

bool Module::Render::Editor::Editor::OpenSceneMenu(const std::string& directory)
{
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_directory()) {
			if (ImGui::BeginMenu(entry.path().filename().string().c_str())) {
				OpenSceneMenu(directory + "\\" + entry.path().filename().string());
				ImGui::EndMenu();
			}
		}
		else if (entry.is_regular_file()) {
			if (ImGui::MenuItem(entry.path().filename().string().c_str())) {
				std::string className = "class " + FindSceneClassNameOnFile(entry.path().string());
				selectedScenePath = entry.path().string();
				if (className != "class ") {
					selectedScene = sceneManager->SetSceneByName(className);
				}
			}
		}
	}
	return true;
}

bool Module::Render::Editor::Editor::SelectServOrClient(ImGuiIO& io)
{

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Choix serveur ou client", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x * 800 / 1920, io.DisplaySize.y * 1026 / 1009));
	ImGui::SetWindowPos(ImVec2(io.DisplaySize.x * 1670 / 1920, io.DisplaySize.y * 900 / 1009));

	if (ImGui::Button("Serveur"))
	{
		engine->SetIsServeurTrue();
	}

	if (ImGui::Button("Client"))
	{
		engine->SetIsServeFalse();
	}

	ImGui::End();
	return true;
}

bool Module::Render::Editor::Editor::Chat(ImGuiIO& io)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Chat", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x * 870 / 1920, io.DisplaySize.y * 190 / 1009));
	ImGui::SetWindowPos(ImVec2(io.DisplaySize.x * 800 / 1920, io.DisplaySize.y * 825 / 1009));

	for (const auto& message : chatMessages)
	{
		ImGui::TextWrapped("%s", message.c_str());
	}

	// Saisie d'un nouveau message
	static char inputBuffer[256] = "";
	ImGui::InputText("Message", inputBuffer, IM_ARRAYSIZE(inputBuffer));

	if (engine->GetIsServe() && engine->GetServe() != nullptr)
	{
		if (engine->GetServe()->WSAPoll() > 0)
		{
			auto messageServe = engine->GetServe()->WSAPoll();
			auto msg = messageServe.get();
			auto chat = msg->as<Reseau::Client::UserData>();
			chat->data.back();
			chatMessages.push_back(std::to_string(chat->data.back()));
			strcpy_s(inputBuffer, std::to_string(chat->data.back()).c_str());
			memset(inputBuffer, 0, sizeof(inputBuffer)); // Effacer le buffer après l'envoi du message
		}
	}
	
	if (engine->GetIsClient() && engine->GetClient() != nullptr)
	{
		if (engine->GetClient()->WSAPoll() > 0)
		{
			auto messageClient = engine->GetServe()->WSAPoll();
			auto msg = messageClient.get();
			msg->as<Reseau::Client::UserData>();
			auto chat = msg->as<Reseau::Client::UserData>();
			chat->data.back();
			chatMessages.push_back(std::to_string(chat->data.back()));
			strcpy_s(inputBuffer, std::to_string(chat->data.back()).c_str());
			memset(inputBuffer, 0, sizeof(inputBuffer)); // Effacer le buffer après l'envoi du message
		}
	}
	
	// Envoi d'un nouveau message
	if (ImGui::Button("Envoyer") && inputBuffer[0] != '\0')
	{
		int result = 0;
		if (engine->GetIsServe())
		{
			engine->GetServe()->SendToAll(reinterpret_cast <const unsigned char*>(inputBuffer), sizeof(inputBuffer));
			chatMessages.push_back(inputBuffer);
			memset(inputBuffer, 0, sizeof(inputBuffer)); // Effacer le buffer après l'envoi du message
		}
		else if (engine->GetIsClient())
		{
			engine->GetClient()->send(reinterpret_cast <const unsigned char*>(inputBuffer), sizeof(inputBuffer));
			chatMessages.push_back(inputBuffer);
			memset(inputBuffer, 0, sizeof(inputBuffer)); // Effacer le buffer après l'envoi du message
		}
		else
		{
			chatMessages.push_back(inputBuffer);
			memset(inputBuffer, 0, sizeof(inputBuffer)); // Effacer le buffer après l'envoi du message
		}
	}
	

	ImGui::End();
	return true;
}


std::string Module::Render::Editor::Editor::FindSceneClassNameOnFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		
	}

	std::regex classDeclarationRegex(R"(^\s*class\s+(\w+)\s*)");
	std::regex sceneClassDeclarationRegex(R"(^\s*class\s+(\w+)\s*:\s*public\s+Core::Interface::IScene\s*)");

	std::string line;
	while (std::getline(file, line)) {
		std::smatch match;
		if (std::regex_search(line, match, classDeclarationRegex)) {
			if (match.size() > 1) {
				if (std::regex_search(line, match, sceneClassDeclarationRegex)) {
					if (match.size() > 1) {
						return match[1];
					}
				}
				return "";
			}
		}
	}

	file.close();

	return "";
}

bool Module::Render::Editor::Editor::CheckMove() 
{
	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		if (inputModule->IsKeyPressed(Module::Input::KeyboardKey::Z))
		{
			activeCamera->GetTransformReference()->MoveForward(0.01 * TimeModule->GetDeltaTime());
		}
		if (inputModule->IsKeyPressed(Module::Input::KeyboardKey::S))
		{
			activeCamera->GetTransformReference()->MoveBackward(0.01 * TimeModule->GetDeltaTime());
		}
		if (inputModule->IsKeyPressed(Module::Input::KeyboardKey::Q))
		{
			activeCamera->GetTransformReference()->MoveLeft(0.01 * TimeModule->GetDeltaTime());
		}
		if (inputModule->IsKeyPressed(Module::Input::KeyboardKey::D))
		{
			activeCamera->GetTransformReference()->MoveRight(0.01 * TimeModule->GetDeltaTime());
		}
		if (inputModule->IsKeyPressed(Module::Input::KeyboardKey::Space))
		{
			activeCamera->GetTransformReference()->MoveUp(0.01 * TimeModule->GetDeltaTime());
		}
		if (inputModule->IsKeyPressed(Module::Input::KeyboardKey::LShift))
		{
			activeCamera->GetTransformReference()->MoveDown(0.01 * TimeModule->GetDeltaTime());
		}
		if (inputModule->isRightButtonDown()) 
		{
			activeCamera->Rotate(inputModule->GetMouseDelta()[0], inputModule->GetMouseDelta()[1]);
		}
	}

	return true;
}

bool Module::Render::Editor::Editor::CheckIfMouseSelectGameObject(ImGuiIO& io)
{
	float mousePosX = inputModule->GetMousePos()[0];
	float mousePosY = inputModule->GetMousePos()[1];
	float screenWidth = io.DisplaySize.x;
	float screenHeight = io.DisplaySize.y;
	D3D12_VIEWPORT* viewport = renderModule->GetRenderer()->GetViewport();
	if (inputModule->isLeftButtonDown() && mousePosX >= viewport->TopLeftX && mousePosX <= viewport->TopLeftX + viewport->Width 
		&& mousePosY >= viewport->TopLeftY && mousePosY <= viewport->TopLeftY + viewport->Height)
	{
		float distance = INFINITE;

		for (Objects::GameObject* gameObject : *selectedScene->GetGameObjects())
		{

			if (gameObject->GetComponent<Objects::Component::GraphicComponent>() != nullptr) 
	  {
				DirectX::XMFLOAT3 position = gameObject->GetTransformReference()->position;
				DirectX::XMFLOAT3 rotation = gameObject->GetTransformReference()->rotation;
				DirectX::XMFLOAT3 scale = gameObject->GetTransformReference()->scale;

				DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixIdentity(); // Initialisation � l'identit�
				modelMatrix *= DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&position)); // Translation
				modelMatrix *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x)); // Rotation autour de l'axe X
				modelMatrix *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y)); // Rotation autour de l'axe Y
				modelMatrix *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z)); // Rotation autour de l'axe Z
				modelMatrix *= DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));

				DirectX::XMFLOAT4X4 temp_proj_mat = renderModule->GetActiveCamera()->GetProjectionMatrix();
				DirectX::XMMATRIX projectionMatrix = DirectX::XMLoadFloat4x4(&temp_proj_mat);
				DirectX::XMStoreFloat4x4(&temp_proj_mat, projectionMatrix);
				DirectX::XMFLOAT4X4 temp_view_mat = renderModule->GetActiveCamera()->GetViewMatrix();
				DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&temp_view_mat);

				DirectX::XMMATRIX MVP = DirectX::XMMatrixMultiply(projectionMatrix, DirectX::XMMatrixMultiply(viewMatrix, modelMatrix));

				DirectX::XMFLOAT4 tempfloat4 = DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f);
				DirectX::XMVECTOR objectCoordsVec = DirectX::XMLoadFloat4(&tempfloat4);
				DirectX::XMVECTOR projectedCoordsVec = DirectX::XMVector4Transform(objectCoordsVec, MVP);

				DirectX::XMFLOAT4 projectedCoords;
				DirectX::XMStoreFloat4(&projectedCoords, projectedCoordsVec);
				float screenX = (projectedCoords.x / 1 + 1.0f) * 0.5f * screenWidth;
				float screenY = (1.0f - projectedCoords.y / 1) * 0.5f * screenHeight;

				DirectX::XMVECTOR objectSizeVec = DirectX::XMLoadFloat3(&scale);
				DirectX::XMVECTOR projectedSizeVec = DirectX::XMVector4Transform(objectSizeVec, projectionMatrix);

				DirectX::XMFLOAT4 projectedSize;
				DirectX::XMStoreFloat4(&projectedSize, projectedSizeVec);

				float objectWidthOnScreen = projectedSize.x / 1 * screenWidth;
				float objectHeightOnScreen = projectedSize.y / 1 * screenHeight;

				DirectX::XMVECTOR nearPoint = DirectX::XMVectorSet(mousePosX, mousePosY, 0.0f, 1.0f);
				DirectX::XMVECTOR farPoint = DirectX::XMVectorSet(mousePosX, mousePosY, 1.0f, 1.0f);

				DirectX::XMFLOAT4X4 worldMatrix = gameObject->GetTransformReference()->GetWorldMatrix();

				nearPoint = DirectX::XMVector3Unproject(nearPoint, viewport->TopLeftX, viewport->TopLeftY, viewport->Width, viewport->Height, viewport->MinDepth, viewport->MaxDepth, projectionMatrix, viewMatrix, DirectX::XMLoadFloat4x4(&worldMatrix));
				farPoint = DirectX::XMVector3Unproject(farPoint, viewport->TopLeftX, viewport->TopLeftY, viewport->Width, viewport->Height, viewport->MinDepth, viewport->MaxDepth, projectionMatrix, viewMatrix, DirectX::XMLoadFloat4x4(&worldMatrix));

				bool isHit = CheckCollisionWith3DElement(nearPoint, farPoint, gameObject->GetComponent<Objects::Component::GraphicComponent>()->GetMesh());

				if (isHit) {
					float distToIntersection = std::sqrt(std::pow(projectedCoords.x - mousePosX, 2) +
						std::pow(projectedCoords.y - mousePosY, 2));
					if (distToIntersection < distance) {
						distance = distToIntersection;
						selectedGameObject = gameObject;
					}
				}
			}
		}
	}
	return true;
}

bool Module::Render::Editor::Editor::CheckCollisionWith3DElement(DirectX::XMVECTOR nearPoint, DirectX::XMVECTOR farPoint,Core::CoreType::Mesh* mesh) 
{
	if (IntersectRayTriangle(nearPoint, farPoint, mesh)){
		return true;
	}
	return false;
}

bool Module::Render::Editor::Editor::IntersectRayTriangle(DirectX::FXMVECTOR rayOrigin, DirectX::FXMVECTOR rayDirection, Core::CoreType::Mesh* mesh) 
{
	for (int i = 0; i < mesh->GetIndexCount(); i += 3)
	{
		DirectX::XMVECTOR v0 = DirectX::XMVectorSet((mesh->GetVertices()[mesh->GetIndexes()[i]]).GetTable()[0], (mesh->GetVertices()[mesh->GetIndexes()[i]]).GetTable()[1], (mesh->GetVertices()[mesh->GetIndexes()[i]]).GetTable()[2], 1);
		DirectX::XMVECTOR v1 = DirectX::XMVectorSet((mesh->GetVertices()[mesh->GetIndexes()[i+1]]).GetTable()[0], (mesh->GetVertices()[mesh->GetIndexes()[i+1]]).GetTable()[1], (mesh->GetVertices()[mesh->GetIndexes()[i+1]]).GetTable()[2], 1);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSet((mesh->GetVertices()[mesh->GetIndexes()[i+2]]).GetTable()[0], (mesh->GetVertices()[mesh->GetIndexes()[i+2]]).GetTable()[1], (mesh->GetVertices()[mesh->GetIndexes()[i+2]]).GetTable()[2], 1);

		// Calculer le vecteur du côté du triangle
		DirectX::XMVECTOR edge1 = DirectX::XMVectorSubtract(v1, v0);
		DirectX::XMVECTOR edge2 = DirectX::XMVectorSubtract(v2, v0);

		// Calculer le produit vectoriel entre le rayon et le second côté du triangle
		DirectX::XMVECTOR pvec = DirectX::XMVector3Cross(rayDirection, edge2);

		// Calculer le déterminant
		float det = DirectX::XMVectorGetX(DirectX::XMVector3Dot(edge1, pvec));

		// Vérifier si le déterminant est proche de zéro (le rayon est parallèle au plan du triangle)
		if (fabs(det) < 0.00001f) {
			return false;
		}

		// Calculer l'inverse du déterminant
		float invDet = 1.0f / det;

		// Calculer le vecteur du côté du triangle depuis l'origine du rayon jusqu'au premier sommet du triangle
		DirectX::XMVECTOR tvec = DirectX::XMVectorSubtract(rayOrigin, v0);

		// Calculer le produit scalaire entre le vecteur côté-triangle et le vecteur côté-rayon
		float u = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tvec, pvec)) * invDet;

		// Vérifier si u est dans la plage [0, 1]
		if (u < 0.0f || u > 1.0f) {
			return false;
		}

		// Calculer le produit vectoriel entre le vecteur côté-rayon et le second côté du triangle
		DirectX::XMVECTOR qvec = DirectX::XMVector3Cross(tvec, edge1);

		// Calculer le produit scalaire entre le produit vectoriel précédent et le rayon
		float v = DirectX::XMVectorGetX(DirectX::XMVector3Dot(rayDirection, qvec)) * invDet;

		// Vérifier si v est dans la plage [0, 1] et si la somme de u et v est inférieure ou égale à 1
		if (v < 0.0f || (u + v) > 1.0f) {
			return false;
		}

		return true;
	}
}
