int main(int argc, char** argv) try {

    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other SDK objects and stays alive until all of them are destroyed.
    viam::sdk::Instance inst;

    // Write general log statements using the VIAM_SDK_LOG macro.
    VIAM_SDK_LOG(info) << "Starting up {{ .ModuleName }} module";

    Model model("{{ .orgID }}", "{{ .ModuleName }}", "{{ .ModelSnake }}");


    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        API::get<>,
        model,
        [](viam::sdk::Dependencies deps, viam::sdk::ResourceConfig cfg) {
            return std::make_unique<{{ .ModelPascal }}>(deps, cfg);
        },
        &{{ .ModelPascal }}::validate);



    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);
    my_mod->serve();

    return EXIT_SUCCESS;
} catch (const viam::sdk::Exception& ex) {
    std::cerr << "main failed with exception: " << ex.what() << "\n";
    return EXIT_FAILURE;
}
