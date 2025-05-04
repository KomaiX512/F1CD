#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h> // Include internal for DockSpaceOverViewport and docking flags
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "CarDesign.h"
#include "ConfigurationManager.h"

void setupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.95f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.15f, 0.95f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.50f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.20f, 0.80f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.30f, 0.80f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.35f, 0.80f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.40f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.50f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.35f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.40f, 0.60f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.50f, 0.70f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.35f, 0.55f, 0.80f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.35f, 0.50f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.20f, 0.80f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.50f, 0.70f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.40f, 0.60f, 1.00f);
    style.WindowRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 8.0f;
    style.TabRounding = 8.0f;
    style.FramePadding = ImVec2(8, 6);
    style.WindowPadding = ImVec2(10, 10);
    style.ItemSpacing = ImVec2(10, 8);
}

void drawCarSilhouette(ImDrawList* drawList, ImVec2 center, float scale) {
    float w = 100.0f * scale, h = 40.0f * scale;
    ImVec2 p1(center.x - w * 0.5f, center.y - h * 0.5f);
    ImVec2 p2(center.x + w * 0.5f, center.y - h * 0.5f);
    ImVec2 p3(center.x + w * 0.5f, center.y + h * 0.5f);
    ImVec2 p4(center.x - w * 0.5f, center.y + h * 0.5f);
    drawList->AddRectFilled(p1, p3, ImColor(0.20f, 0.20f, 0.25f, 0.90f), 8.0f);
    drawList->AddRect(p1, p3, ImColor(0.80f, 0.20f, 0.20f, 1.00f), 8.0f, 0, 2.0f);
    drawList->AddCircleFilled(ImVec2(p1.x + w * 0.2f, p4.y), h * 0.25f, ImColor(0.10f, 0.10f, 0.10f));
    drawList->AddCircleFilled(ImVec2(p2.x - w * 0.2f, p4.y), h * 0.25f, ImColor(0.10f, 0.10f, 0.10f));
}

void drawGauge(ImDrawList* drawList, ImVec2 center, float radius, float value, float maxValue, const char* label) {
    float angle = (value / maxValue) * 1.5f * 3.14159f - 0.75f * 3.14159f;
    drawList->AddCircleFilled(center, radius, ImColor(0.15f, 0.15f, 0.20f));
    drawList->AddCircle(center, radius, ImColor(0.80f, 0.20f, 0.20f), 12, 2.0f);
    ImVec2 needleEnd(center.x + radius * 0.8f * cos(angle), center.y + radius * 0.8f * sin(angle));
    drawList->AddLine(center, needleEnd, ImColor(0.90f, 0.90f, 0.95f), 2.0f);
    ImGui::SetCursorPos(ImVec2(center.x - ImGui::CalcTextSize(label).x * 0.5f, center.y + radius + 10));
    ImGui::Text("%s: %.2f", label, value);
}

#include "ConfigurationManager.h" // Add include for ensureDesignsDirectory

int main() {
    std::cout << "Application started." << std::endl;

    try {
        // Initialize ConfigurationManager and ensure designs directory exists
        ConfigurationManager configManager;
        configManager.ensureDesignsDirectory();

        // Log the number of design files
        int designCount = configManager.countDesignFiles("designs");
        std::cout << "Number of design files: " << designCount << std::endl;

        std::cout << "Starting main()" << std::endl;

        // Ensure designs directory exists before starting
        ConfigurationManager::ensureDesignsDirectory();
        std::cout << "Ensured designs directory" << std::endl;

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }
        std::cout << "GLFW initialized" << std::endl;

        GLFWwindow* window = glfwCreateWindow(1600, 900, "Formula One Car Designer", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return -1;
        }
        std::cout << "GLFW window created" << std::endl;

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.Fonts->AddFontFromFileTTF("../fonts/Roboto/static/Roboto-Bold.ttf", 20.0f); // Assume a bold font is available
        if (io.Fonts->Fonts.Size == 0) io.Fonts->AddFontDefault();
        setupImGuiStyle();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        enum class Section { WELCOME, MAIN_MENU, DESIGN, LOAD, COMPARE };
        Section currentSection = Section::WELCOME;
        CarDesign currentDesign;
        CarDesign compareDesign1, compareDesign2;
        std::vector<std::string> designFiles;
        int selections[4] = {0, 0, 0, 0};
        float aeroAdjustments[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        char filename[128] = "";
        int selectedDesignIndex = -1;
        int compareDesignIndex1 = -1, compareDesignIndex2 = -1;
        bool showError = false, showConfirm = false, showSaveSuccess = false;
        std::string errorMessage;
        bool overwriteConfirmed = false;
        bool designsDirError = false;
        std::string nameValidationMessage;
        bool isNameValid = false;
        float sectionAlpha = 0.0f;
        float welcomeTime = 0.0f;
        float saveProgress = 0.0f;

        const char* designNames[] = {"Standard", "High Downforce", "Low Drag", "Balanced", "Experimental"};
        const char* diffuserNames[] = {"Standard", "Aggressive", "Minimal", "Balanced", "Experimental"};
        const char* sidepodsNames[] = {"Standard", "Compact", "Streamlined", "Balanced", "Experimental"};

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            // Animation for section transitions
            float deltaTime = ImGui::GetIO().DeltaTime;
            sectionAlpha = std::min(sectionAlpha + deltaTime * 2.0f, 1.0f);

            if (showError) {
                ImGui::OpenPopup("Error");
                ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: %s", errorMessage.c_str());
                if (ImGui::Button("OK")) showError = false;
                ImGui::EndPopup();
            }

            if (showConfirm) {
                ImGui::OpenPopup("Confirm Overwrite");
                ImGui::BeginPopupModal("Confirm Overwrite", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("Design '%s' already exists. Overwrite?", filename);
                if (ImGui::Button("Yes")) {
                    overwriteConfirmed = true;
                    showConfirm = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("No")) showConfirm = false;
                ImGui::EndPopup();
            }

            if (showSaveSuccess) {
                saveProgress = std::min(saveProgress + deltaTime * 2.0f, 1.0f);
                ImGui::OpenPopup("Save Success");
                ImGui::BeginPopupModal("Save Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("Design '%s' saved successfully!", filename);
                ImGui::ProgressBar(saveProgress, ImVec2(300, 20));
                if (saveProgress >= 1.0f && ImGui::Button("OK")) {
                    showSaveSuccess = false;
                    saveProgress = 0.0f;
                }
                ImGui::EndPopup();
            }

            if (currentSection == Section::WELCOME) {
                welcomeTime += deltaTime;
                float alpha = std::sin(welcomeTime * 1.5f) * 0.5f + 0.5f;
                ImGui::Begin("Welcome", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::SetWindowPos(ImVec2(io.DisplaySize.x * 0.5f - 300, io.DisplaySize.y * 0.5f - 100));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.20f, 0.20f, alpha));
                ImGui::TextColored(ImVec4(0.80f, 0.20f, 0.20f, alpha), "Formula One Car Designer");
                ImGui::PopStyleColor();
                ImGui::Text("Created for F1 Enthusiasts");
                if (welcomeTime > 3.0f) {
                    currentSection = Section::MAIN_MENU;
                    sectionAlpha = 0.0f;
                }
                ImGui::End();
            } else {
                // Sidebar for navigation
                ImGui::Begin("Navigation", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                ImGui::SetWindowPos(ImVec2(0, 0));
                ImGui::SetWindowSize(ImVec2(250, io.DisplaySize.y));
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.10f, 0.15f, 0.95f));
                ImGui::BeginChild("NavButtons", ImVec2(0, 0), true);
                ImGui::TextColored(ImVec4(0.80f, 0.20f, 0.20f, 1.0f), "F1 Car Designer");
                ImGui::Separator();
                if (ImGui::Button("Design New Car", ImVec2(200, 50))) {
                    currentSection = Section::DESIGN;
                    std::fill_n(selections, 4, 0);
                    std::fill_n(aeroAdjustments, 4, 1.0f);
                    filename[0] = '\0';
                    nameValidationMessage.clear();
                    isNameValid = false;
                    sectionAlpha = 0.0f;
                }
                if (ImGui::Button("Load Configuration", ImVec2(200, 50))) {
                try {
                    designFiles = ConfigurationManager::getDesignFiles();
                    currentSection = Section::LOAD;
                    selectedDesignIndex = -1;
                    sectionAlpha = 0.0f;
                    showError = false;
                    designsDirError = false;
                } catch (const std::exception& e) {
                    showError = true;
                    errorMessage = e.what();
                    designsDirError = std::string(e.what()).find("designs") != std::string::npos;
                }
                }
                if (ImGui::Button("Compare Configurations", ImVec2(200, 50))) {
                try {
                    designFiles = ConfigurationManager::getDesignFiles();
                    currentSection = Section::COMPARE;
                    compareDesignIndex1 = compareDesignIndex2 = -1;
                    sectionAlpha = 0.0f;
                    showError = false;
                    designsDirError = false;
                } catch (const std::exception& e) {
                    showError = true;
                    errorMessage = e.what();
                    designsDirError = true;
                }
                }
                if (ImGui::Button("Exit", ImVec2(200, 50))) glfwSetWindowShouldClose(window, true);
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::End();

                if (currentSection == Section::MAIN_MENU) {
                    ImGui::Begin("Dashboard", nullptr, ImGuiWindowFlags_NoMove);
                    ImGui::SetWindowPos(ImVec2(250, 0));
                    ImGui::SetWindowSize(ImVec2(io.DisplaySize.x - 250, io.DisplaySize.y));
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sectionAlpha);
                    ImGui::TextColored(ImVec4(0.80f, 0.20f, 0.20f, sectionAlpha), "Welcome to the F1 Car Designer");
                    ImGui::Separator();
                    ImGui::TextWrapped("Design and compare Formula 1 car configurations with real-time metrics and visualizations.");
                    ImGui::Dummy(ImVec2(0, 20));
                    ImGui::Text("Quick Stats:");
                    int designCount = 0;
                    try {
                        designCount = ConfigurationManager::countDesignFiles("designs");
                    } catch (const std::exception& e) {
                        designsDirError = true;
                    }
                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Designs Saved: %d", designCount);
                    if (designsDirError) {
                        ImGui::TextColored(ImVec4(1, 0, 0, sectionAlpha), "Warning: Unable to access designs directory");
                    }
                    ImGui::Dummy(ImVec2(0, 20));
                    drawCarSilhouette(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 300, ImGui::GetCursorScreenPos().y + 100), 1.5f);
                    ImGui::PopStyleVar();
                    ImGui::End();
                } else if (currentSection == Section::DESIGN) {
                    ImGui::Begin("Design Car", nullptr, ImGuiWindowFlags_NoMove);
                    ImGui::SetWindowPos(ImVec2(250, 0));
                    ImGui::SetWindowSize(ImVec2(io.DisplaySize.x - 250, io.DisplaySize.y));
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sectionAlpha);
                    ImGui::TextColored(ImVec4(0.80f, 0.20f, 0.20f, sectionAlpha), "Design Your Formula One Car");
                    ImGui::Separator();

                    CarDesign previewDesign;
                    try {
                        previewDesign.setPartDesign(FRONT_WING, selections[0]);
                        previewDesign.setPartDesign(REAR_WING, selections[1]);
                        previewDesign.setPartDesign(DIFFUSER, selections[2]);
                        previewDesign.setPartDesign(SIDEPODS, selections[3]);
                        previewDesign.adjustAeroEfficiency(FRONT_WING, aeroAdjustments[0]);
                        previewDesign.adjustAeroEfficiency(REAR_WING, aeroAdjustments[1]);
                        previewDesign.adjustAeroEfficiency(DIFFUSER, aeroAdjustments[2]);
                        previewDesign.adjustAeroEfficiency(SIDEPODS, aeroAdjustments[3]);
                    } catch (const std::exception& e) {
                        showError = true;
                        errorMessage = e.what();
                    }

                    if (ImGui::BeginTabBar("DesignTabs")) {
                        if (ImGui::BeginTabItem("Components")) {
                            if (ImGui::CollapsingHeader("Front Wing", ImGuiTreeNodeFlags_DefaultOpen)) {
                                ImGui::Combo("Design##FrontWing", &selections[0], designNames, IM_ARRAYSIZE(designNames));
                                ImGui::SliderFloat("Aero Efficiency##FrontWing", &aeroAdjustments[0], 0.5f, 1.5f, "%.2f");
                                FrontWing fw; fw.setDesign(selections[0]); fw.adjustAeroEfficiency(aeroAdjustments[0]);
                                ImGui::Text("Drag: %.2f, Mass: %.2f kg, Cost: $%.2f", 
                                            fw.getAttributes().drag, fw.getAttributes().mass, fw.getAttributes().cost);
                            }
                            if (ImGui::CollapsingHeader("Rear Wing", ImGuiTreeNodeFlags_DefaultOpen)) {
                                ImGui::Combo("Design##RearWing", &selections[1], designNames, IM_ARRAYSIZE(designNames));
                                ImGui::SliderFloat("Aero Efficiency##RearWing", &aeroAdjustments[1], 0.5f, 1.5f, "%.2f");
                                RearWing rw; rw.setDesign(selections[1]); rw.adjustAeroEfficiency(aeroAdjustments[1]);
                                ImGui::Text("Drag: %.2f, Mass: %.2f kg, Cost: $%.2f", 
                                            rw.getAttributes().drag, rw.getAttributes().mass, rw.getAttributes().cost);
                            }
                            if (ImGui::CollapsingHeader("Diffuser", ImGuiTreeNodeFlags_DefaultOpen)) {
                                ImGui::Combo("Design##Diffuser", &selections[2], diffuserNames, IM_ARRAYSIZE(diffuserNames));
                                ImGui::SliderFloat("Aero Efficiency##Diffuser", &aeroAdjustments[2], 0.5f, 1.5f, "%.2f");
                                Diffuser df; df.setDesign(selections[2]); df.adjustAeroEfficiency(aeroAdjustments[2]);
                                ImGui::Text("Drag: %.2f, Mass: %.2f kg, Cost: $%.2f", 
                                            df.getAttributes().drag, df.getAttributes().mass, df.getAttributes().cost);
                            }
                            if (ImGui::CollapsingHeader("Sidepods", ImGuiTreeNodeFlags_DefaultOpen)) {
                                ImGui::Combo("Design##Sidepods", &selections[3], sidepodsNames, IM_ARRAYSIZE(sidepodsNames));
                                ImGui::SliderFloat("Aero Efficiency##Sidepods", &aeroAdjustments[3], 0.5f, 1.5f, "%.2f");
                                Sidepods sp; sp.setDesign(selections[3]); sp.adjustAeroEfficiency(aeroAdjustments[3]);
                                ImGui::Text("Drag: %.2f, Mass: %.2f kg, Cost: $%.2f", 
                                            sp.getAttributes().drag, sp.getAttributes().mass, sp.getAttributes().cost);
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Metrics")) {
                            auto attrs = previewDesign.getTotalAttributes();
                            ImGui::BeginChild("MetricsChild", ImVec2(0, 300));
                            drawGauge(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 150, ImGui::GetCursorScreenPos().y + 150), 80.0f, 
                                      previewDesign.getSpeed(), 400.0f, "Speed (km/h)");
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 300);
                            drawGauge(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 150, ImGui::GetCursorScreenPos().y + 150), 80.0f, 
                                      previewDesign.getFuelConsumption(), 20.0f, "Fuel (L/100km)");
                            ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Drag: %.2f", attrs.drag);
                            ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Mass: %.2f kg", attrs.mass);
                            ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Cost: $%.2f", attrs.cost);
                            ImGui::EndChild();
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Preview")) {
                            drawCarSilhouette(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 300, ImGui::GetCursorScreenPos().y + 150), 2.0f);
                            ImGui::TextWrapped("Visual Representation:\n%s", previewDesign.getVisualRepresentation().c_str());
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }

                    ImGui::Separator();
                    std::string inputName(filename);
                    auto [valid, validationError] = CarDesign::validateDesignName(inputName);
                    isNameValid = valid;
                    nameValidationMessage = validationError.empty() ? "Valid" : validationError;

                    ImGui::PushStyleColor(ImGuiCol_FrameBg, isNameValid || inputName.empty() ? 
                                          ImVec4(0.15f, 0.15f, 0.20f, 0.80f) : ImVec4(0.50f, 0.10f, 0.10f, 0.80f));
                    ImGui::InputText("Design Name", filename, IM_ARRAYSIZE(filename));
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::TextColored(isNameValid ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%s", nameValidationMessage.c_str());

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sectionAlpha);
                    bool isHovered = ImGui::IsItemHovered();
                    float buttonScale = isHovered ? 1.1f : 1.0f;
                    if (ImGui::Button("Save Design", ImVec2(150 * buttonScale, 40 * buttonScale))) {
                        try {
                            std::string cleanName(filename);
                            cleanName.erase(std::remove_if(cleanName.begin(), cleanName.end(), 
                                                           [](char c) { return c < 0 || c > 127; }), cleanName.end());
                            auto [isValid, error] = CarDesign::validateDesignName(cleanName);
                            if (!isValid) {
                                throw std::invalid_argument(error);
                            }
                            if (ConfigurationManager::designExists(cleanName) && !overwriteConfirmed) {
                                showConfirm = true;
                            } else {
                                ConfigurationManager::backupDesign(cleanName);
                                currentDesign.setPartDesign(FRONT_WING, selections[0]);
                                currentDesign.setPartDesign(REAR_WING, selections[1]);
                                currentDesign.setPartDesign(DIFFUSER, selections[2]);
                                currentDesign.setPartDesign(SIDEPODS, selections[3]);
                                currentDesign.adjustAeroEfficiency(FRONT_WING, aeroAdjustments[0]);
                                currentDesign.adjustAeroEfficiency(REAR_WING, aeroAdjustments[1]);
                                currentDesign.adjustAeroEfficiency(DIFFUSER, aeroAdjustments[2]);
                                currentDesign.adjustAeroEfficiency(SIDEPODS, aeroAdjustments[3]);
                                currentDesign.saveToFile(cleanName);
                                overwriteConfirmed = false;
                                showError = false;
                                showSaveSuccess = true;
                                currentSection = Section::MAIN_MENU;
                                sectionAlpha = 0.0f;
                            }
                        } catch (const std::exception& e) {
                            showError = true;
                            errorMessage = e.what();
                            designsDirError = std::string(e.what()).find("designs") != std::string::npos;
                        }
                    }
                    ImGui::SameLine();
                    isHovered = ImGui::IsItemHovered();
                    buttonScale = isHovered ? 1.1f : 1.0f;
    if (ImGui::Button("Back", ImVec2(150 * buttonScale, 40 * buttonScale))) {
        currentSection = Section::MAIN_MENU;
        sectionAlpha = 0.0f;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(); // Added extra PopStyleVar to fix imbalance
    ImGui::End();
                } else if (currentSection == Section::LOAD) {
                    ImGui::Begin("Load Configuration", nullptr, ImGuiWindowFlags_NoMove);
                    ImGui::SetWindowPos(ImVec2(250, 0));
                    ImGui::SetWindowSize(ImVec2(io.DisplaySize.x - 250, io.DisplaySize.y));
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sectionAlpha);
                    ImGui::TextColored(ImVec4(0.80f, 0.20f, 0.20f, sectionAlpha), "Select a Saved Design:");
                    ImGui::BeginChild("DesignList", ImVec2(0, 400), true);
                    for (size_t i = 0; i < designFiles.size(); ++i) {
                        if (ImGui::Selectable(designFiles[i].c_str(), selectedDesignIndex == i)) {
                            selectedDesignIndex = i;
                            try {
                                currentDesign.loadFromFile(designFiles[i]);
                                showError = false;
                            } catch (const std::exception& e) {
                                showError = true;
                                errorMessage = e.what();
                                designsDirError = std::string(e.what()).find("designs") != std::string::npos;
                            }
                        }
                    }
                    ImGui::EndChild();
                    if (selectedDesignIndex >= 0 && !showError) {
                        ImGui::Separator();
                        ImGui::Text("Design: %s", designFiles[selectedDesignIndex].c_str());
                        drawCarSilhouette(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 300, ImGui::GetCursorScreenPos().y + 150), 2.0f);
                        ImGui::TextWrapped("Visual Representation:\n%s", currentDesign.getVisualRepresentation().c_str());
                        ImGui::Separator();
                        auto attrs = currentDesign.getTotalAttributes();
                        ImGui::BeginChild("MetricsChild", ImVec2(0, 300));
                        drawGauge(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 150, ImGui::GetCursorScreenPos().y + 150), 80.0f, 
                                  currentDesign.getSpeed(), 400.0f, "Speed (km/h)");
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 300);
                        drawGauge(ImGui::GetWindowDrawList(), ImVec2(ImGui::GetCursorScreenPos().x + 150, ImGui::GetCursorScreenPos().y + 150), 80.0f, 
                                  currentDesign.getFuelConsumption(), 20.0f, "Fuel (L/100km)");
                        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Drag: %.2f", attrs.drag);
                        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Mass: %.2f kg", attrs.mass);
                        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, sectionAlpha), "Total Cost: $%.2f", attrs.cost);
                        ImGui::EndChild();
                    }
                    bool isHovered = ImGui::IsItemHovered();
                    float buttonScale = isHovered ? 1.1f : 1.0f;
                    if (ImGui::Button("Back", ImVec2(150 * buttonScale, 40 * buttonScale))) {
                        currentSection = Section::MAIN_MENU;
                        sectionAlpha = 0.0f;
                    }
                    ImGui::PopStyleVar();
                    ImGui::End();
                } else if (currentSection == Section::COMPARE) {
                    ImGui::Begin("Compare Configurations", nullptr, ImGuiWindowFlags_NoMove);
                    ImGui::SetWindowPos(ImVec2(250, 0));
                    ImGui::SetWindowSize(ImVec2(io.DisplaySize.x - 250, io.DisplaySize.y));
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sectionAlpha);
                    ImGui::TextColored(ImVec4(0.80f, 0.20f, 0.20f, sectionAlpha), "Select Designs to Compare:");
                    ImGui::BeginChild("DesignList1", ImVec2(0, 200), true);
                    ImGui::Text("First Design:");
                    for (size_t i = 0; i < designFiles.size(); ++i) {
                        if (ImGui::Selectable((designFiles[i] + " (1)").c_str(), compareDesignIndex1 == i)) {
                            if (i != compareDesignIndex2) {
                                compareDesignIndex1 = i;
                                try {
                                    compareDesign1 = CarDesign();
                                    compareDesign1.loadFromFile(designFiles[i]);
                                    showError = false;
                                } catch (const std::exception& e) {
                                    showError = true;
                                    errorMessage = e.what();
                                    designsDirError = std::string(e.what()).find("designs") != std::string::npos;
                                }
                            } else {
                                showError = true;
                                errorMessage = "Cannot select the same design twice";
                            }
                        }
                    }
                    ImGui::EndChild();
                    ImGui::BeginChild("DesignList2", ImVec2(0, 200), true);
                    ImGui::Text("Second Design:");
                    for (size_t i = 0; i < designFiles.size(); ++i) {
                        if (ImGui::Selectable((designFiles[i] + " (2)").c_str(), compareDesignIndex2 == i)) {
                            if (i != compareDesignIndex1) {
                                compareDesignIndex2 = i;
                                try {
                                    compareDesign2 = CarDesign();
                                    compareDesign2.loadFromFile(designFiles[i]);
                                    showError = false;
                                } catch (const std::exception& e) {
                                    showError = true;
                                    errorMessage = e.what();
                                    designsDirError = std::string(e.what()).find("designs") != std::string::npos;
                                }
                            } else {
                                showError = true;
                                errorMessage = "Cannot select the same design twice";
                            }
                        }
                    }
                    ImGui::EndChild();
                    if (compareDesignIndex1 >= 0 && compareDesignIndex2 >= 0 && !showError) {
                        ImGui::Separator();
                        ImGui::Text("Comparison: %s vs %s", designFiles[compareDesignIndex1].c_str(), designFiles[compareDesignIndex2].c_str());
                        ImGui::Text("Design 1 Details:");
                        ImGui::Text("Front Wing: %s (Aero: %.2f)", 
                                    compareDesign1.getPartDesignName(FRONT_WING).c_str(), compareDesign1.getAeroEfficiency(FRONT_WING));
                        ImGui::Text("Rear Wing: %s (Aero: %.2f)", 
                                    compareDesign1.getPartDesignName(REAR_WING).c_str(), compareDesign1.getAeroEfficiency(REAR_WING));
                        ImGui::Text("Diffuser: %s (Aero: %.2f)", 
                                    compareDesign1.getPartDesignName(DIFFUSER).c_str(), compareDesign1.getAeroEfficiency(DIFFUSER));
                        ImGui::Text("Sidepods: %s (Aero: %.2f)", 
                                    compareDesign1.getPartDesignName(SIDEPODS).c_str(), compareDesign1.getAeroEfficiency(SIDEPODS));
                        ImGui::Text("Design 2 Details:");
                        ImGui::Text("Front Wing: %s (Aero: %.2f)", 
                                    compareDesign2.getPartDesignName(FRONT_WING).c_str(), compareDesign2.getAeroEfficiency(FRONT_WING));
                        ImGui::Text("Rear Wing: %s (Aero: %.2f)", 
                                    compareDesign2.getPartDesignName(REAR_WING).c_str(), compareDesign2.getAeroEfficiency(REAR_WING));
                        ImGui::Text("Diffuser: %s (Aero: %.2f)", 
                                    compareDesign2.getPartDesignName(DIFFUSER).c_str(), compareDesign2.getAeroEfficiency(DIFFUSER));
                        ImGui::Text("Sidepods: %s (Aero: %.2f)", 
                                    compareDesign2.getPartDesignName(SIDEPODS).c_str(), compareDesign2.getAeroEfficiency(SIDEPODS));
                        ImGui::Separator();
                        ImGui::BeginTable("ComparisonTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit);
                        ImGui::TableSetupColumn("Metric");
                        ImGui::TableSetupColumn("Design 1");
                        ImGui::TableSetupColumn("Design 2");
                        ImGui::TableSetupColumn("Difference");
                        ImGui::TableHeadersRow();

                        auto attrs1 = compareDesign1.getTotalAttributes();
                        auto attrs2 = compareDesign2.getTotalAttributes();
                        auto fc1 = compareDesign1.getFuelConsumption();
                        auto fc2 = compareDesign2.getFuelConsumption();
                        auto spd1 = compareDesign1.getSpeed();
                        auto spd2 = compareDesign2.getSpeed();

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("Drag");
                        ImGui::TableSetColumnIndex(1); ImGui::TextColored(attrs1.drag <= attrs2.drag ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f", attrs1.drag);
                        ImGui::TableSetColumnIndex(2); ImGui::TextColored(attrs2.drag <= attrs1.drag ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f", attrs2.drag);
                        ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f%%", attrs2.drag != 0 ? (attrs1.drag - attrs2.drag) / attrs2.drag * 100 : 0.0);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("Mass");
                        ImGui::TableSetColumnIndex(1); ImGui::TextColored(attrs1.mass <= attrs2.mass ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f kg", attrs1.mass);
                        ImGui::TableSetColumnIndex(2); ImGui::TextColored(attrs2.mass <= attrs1.mass ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f kg", attrs2.mass);
                        ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f%%", attrs2.mass != 0 ? (attrs1.mass - attrs2.mass) / attrs2.mass * 100 : 0.0);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("Cost");
                        ImGui::TableSetColumnIndex(1); ImGui::TextColored(attrs1.cost <= attrs2.cost ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "$%.2f", attrs1.cost);
                        ImGui::TableSetColumnIndex(2); ImGui::TextColored(attrs2.cost <= attrs1.cost ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "$%.2f", attrs2.cost);
                        ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f%%", attrs2.cost != 0 ? (attrs1.cost - attrs2.cost) / attrs2.cost * 100 : 0.0);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("Fuel Consumption");
                        ImGui::TableSetColumnIndex(1); ImGui::TextColored(fc1 <= fc2 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f L/100km", fc1);
                        ImGui::TableSetColumnIndex(2); ImGui::TextColored(fc2 <= fc1 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f L/100km", fc2);
                        ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f%%", fc2 != 0 ? (fc1 - fc2) / fc2 * 100 : 0.0);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("Speed");
                        ImGui::TableSetColumnIndex(1); ImGui::TextColored(spd1 >= spd2 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f km/h", spd1);
                        ImGui::TableSetColumnIndex(2); ImGui::TextColored(spd2 >= spd1 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), "%.2f km/h", spd2);
                        ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f%%", spd2 != 0 ? (spd1 - spd2) / spd2 * 100 : 0.0);

                        ImGui::EndTable();
                    }
                    bool isHovered = ImGui::IsItemHovered();
                    float buttonScale = isHovered ? 1.1f : 1.0f;
                    if (ImGui::Button("Back", ImVec2(150 * buttonScale, 40 * buttonScale))) {
                        currentSection = Section::MAIN_MENU;
                        sectionAlpha = 0.0f;
                    }
                    ImGui::PopStyleVar();
                    ImGui::End();
                }
            }

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
        std::cout << "Application finished successfully." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
}