#ifndef QUICKSTARTGAME_HPP
#define QUICKSTARTGAME_HPP

#include <Pomdog/Pomdog.hpp>

namespace QuickStart {

using namespace Pomdog;

class QuickStartGame : public Game {
public:
    explicit QuickStartGame(const std::shared_ptr<GameHost>& gameHost);

    void Initialize() override;

    void Update() override;

    void Draw() override;

private:
    std::shared_ptr<GameHost> gameHost;
    std::shared_ptr<GameWindow> window;
    std::shared_ptr<GraphicsDevice> graphicsDevice;
    std::shared_ptr<AssetManager> assets;
    std::shared_ptr<GameClock> clock;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<PipelineState> pipelineState;
    std::shared_ptr<SamplerState> sampler;
    std::shared_ptr<ConstantBufferBinding> constantBuffers;
    std::shared_ptr<ConstantBuffer> constantBuffer;
    std::shared_ptr<Texture2D> texture;
    std::shared_ptr<GraphicsCommandQueue> commandQueue;
    std::shared_ptr<GraphicsCommandList> commandList;
    std::unique_ptr<Timer> timer;
    ConnectionList connect;
};

} // namespace QuickStart

#endif // QUICKSTARTGAME_HPP
