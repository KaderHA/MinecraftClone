#pragma once

int main() {
    auto app = ts::CreateApplication();
    app->Run();
    delete app;
}