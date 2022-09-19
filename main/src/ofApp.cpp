#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(ofEventArgs& args){
}
//--------------------------------------------------------------
void ofApp::update(ofEventArgs& args) {
}
//--------------------------------------------------------------
void ofApp::draw(ofEventArgs & args) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpace", 0, window_flags);
    ImGui::DockSpace(ImGui::GetID("DockSpace"));
    ImGui::PopStyleVar(2);
    ImGui::End();



    ImGui::ShowDemoWindow();
}
//--------------------------------------------------------------
void ofApp::exit(ofEventArgs& args) {
}