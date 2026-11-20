import std;
import vulkan;

int main()
{
    try {
        vulkan::Application application{ vulkan::executable_directory() / "shaders" };
        return application.Run();
    } catch (std::exception const& exception) {
        std::cerr << "Fatal error: " << exception.what() << '\n';
        return 1;
    }
}
