#include <algorithm>
#include <chrono>
#include <filesystem>
#include <span>
#include <string>
#include <thread>

#include <OGP/Game.hpp>
#include <OGP/InputSystem/IInputHandler.hpp>
#include <OGP/InputSystem/InputEvent.hpp>
#include <OGP/Math/Ratio.hpp>
#include <OGP/Rendering/IRenderer.hpp>
#include <OGP/Rendering/RenderingContext.hpp>
#include <OGP/SceneManagement/Node.hpp>

using namespace OGP;
using namespace OGP::InputSystem;
using namespace OGP::Math;
using namespace OGP::Rendering;
using namespace OGP::SceneManagement;
using namespace std;
using namespace std::chrono;
using namespace std::filesystem;
using namespace std::this_thread;

const Ratio<unsigned int> defaultTargetGameTickRate(120U);
const Ratio<unsigned int> defaultTargetFrameRate(60U);

Game::Game(const span<const string> commandLineArguments) :
	targetGameTickRate(defaultTargetGameTickRate),
	targetFrameRate(defaultTargetFrameRate),
	isGameRunning(false),
	gameStartTimePoint(high_resolution_clock::time_point::min()),
	exitCode(-1) {
	// ...
}

Game::Game(const span<const string> commandLineArguments, const path& configurationFilePath) : Game(commandLineArguments) {
	// TODO: Parse configuration file
}

Game::~Game() {
	ClearScenes();
}

bool Game::IsGameRunning() const noexcept {
	return isGameRunning;
}

high_resolution_clock::time_point Game::GetGameStartTimePoint() const noexcept {
	return gameStartTimePoint;
}

const std::vector<InputEvent>& Game::GetCurrentInputEvents() const noexcept {
	return currentInputEvents;
}

vector<InputEvent>& Game::GetCurrentInputEvents(vector<InputEvent>& result) const {
	return result = currentInputEvents;
}

const vector<shared_ptr<Node>>& Game::GetScenes() const noexcept {
	return scenes;
}

vector<shared_ptr<Node>>& Game::GetScenes(vector<shared_ptr<Node>>& result) const {
	return result = scenes;
}

const vector<RenderingContext>& Game::GetRenderingContexts() const noexcept {
	return renderingContexts;
}

vector<RenderingContext>& Game::GetRenderingContexts() noexcept {
	return renderingContexts;
}

int Game::Start() {
	if (!isGameRunning) {
		isGameRunning = true;
		high_resolution_clock::duration target_game_tick_time(
			nanoseconds(static_cast<long long>(targetGameTickRate.denominator) * 1000000000LL / static_cast<long long>(targetGameTickRate.numerator))
		);
		high_resolution_clock::duration target_frame_render_time(
			nanoseconds(static_cast<long long>(targetFrameRate.denominator) * 1000000000LL / static_cast<long long>(targetFrameRate.numerator))
		);
		high_resolution_clock::duration game_tick_time_debt(high_resolution_clock::duration::zero());
		high_resolution_clock::duration frame_render_time_debt(high_resolution_clock::duration::zero());
		high_resolution_clock::time_point now(high_resolution_clock::now());
		high_resolution_clock::time_point last_game_tick_time(now);
		high_resolution_clock::time_point last_rendered_frame_tick_time(now);
		while (isGameRunning) {
			now = high_resolution_clock::now();
			high_resolution_clock::duration game_tick_time(now - last_game_tick_time);
			if ((game_tick_time + game_tick_time_debt) >= target_game_tick_time) {
				last_game_tick_time = now;
				game_tick_time_debt += game_tick_time - target_game_tick_time;
				currentInputEvents.clear();
				for (const auto& input_handler : inputHandlers) {
					input_handler->FetchInputEvents(currentInputEvents);
				}
				for (const auto& input_event : currentInputEvents) {
					if (input_event.GetNameHash() == InputEvent::GetCloseWindowInputEvent().GetNameHash()) {
						Stop();
						break;
					}
				}
				for (const auto& scene : scenes) {
					scene->GameTickScripts(*this, game_tick_time);
				}
			}
			now = high_resolution_clock::now();
			high_resolution_clock::duration frame_render_time(now - last_rendered_frame_tick_time);
			if ((frame_render_time + frame_render_time_debt) >= target_frame_render_time) {
				last_rendered_frame_tick_time = now;
				frame_render_time_debt += frame_render_time - target_frame_render_time;
				for (const auto& scene : scenes) {
					scene->BeforeFrameRenderScripts(*this, frame_render_time);
				}
				for (const auto& scene : scenes) {
					scene->FrameRenderScripts(*this, frame_render_time);
				}
				for (auto& rendering_context : renderingContexts) {
					rendering_context.CommitElements();
					for (auto& renderer : renderers) {
						renderer->Render(rendering_context, frame_render_time);
					}
					rendering_context.Clear();
				}
			}
			now = high_resolution_clock::now();
			high_resolution_clock::duration remaining_game_tick_sleep_time(target_game_tick_time - (now - last_game_tick_time) - game_tick_time_debt);
			high_resolution_clock::duration remaining_frame_render_sleep_time(target_frame_render_time - (now - last_rendered_frame_tick_time) - frame_render_time_debt);
			high_resolution_clock::duration sleep_time(max(min(remaining_game_tick_sleep_time, remaining_frame_render_sleep_time), high_resolution_clock::duration::zero()));
			if (sleep_time > high_resolution_clock::duration::zero()) {
				sleep_for(sleep_time);
			}
		}
	}
	return exitCode;
}

void Game::Stop() {
	Stop(0);
}

void Game::Stop(int exitCode) {
	if (isGameRunning) {
		this->exitCode = exitCode;
		isGameRunning = false;
	}
}

shared_ptr<Node> Game::CreateNewEmptyScene() {
	return CreateNewScene<Node>();
}

bool Game::RemoveScene(const shared_ptr<Node>& scene) {
	const auto& it(find(scenes.begin(), scenes.end(), scene));
	bool ret(it != scenes.end());
	if (ret) {
		scene->Destroy();
		scene->FrameRenderScripts(*this, high_resolution_clock::duration::zero());
		scenes.erase(it);
	}
	return ret;
}

void Game::ClearScenes() {
	for (const auto& scene : scenes) {
		scene->Destroy();
		scene->FrameRenderScripts(*this, high_resolution_clock::duration::zero());
	}
	scenes.clear();
}

bool Game::AddRenderer(const shared_ptr<IRenderer>& renderer) {
	bool ret(renderer && (find(renderers.begin(), renderers.end(), renderer) == renderers.end()));
	if (ret) {
		renderers.push_back(renderer);
	}
	return ret;
}

bool Game::RemoveRenderer(const shared_ptr<IRenderer>& renderer) {
	size_t old_size(renderers.size());
	renderers.erase(remove(renderers.begin(), renderers.end(), renderer), renderers.end());
	return old_size != renderers.size();
}

void Game::ClearRenderers() noexcept {
	renderers.clear();
}

bool Game::AddInputHandler(const shared_ptr<IInputHandler>& inputHandler) {
	bool ret(inputHandler && (find(inputHandlers.begin(), inputHandlers.end(), inputHandler) == inputHandlers.end()));
	if (ret) {
		inputHandlers.push_back(inputHandler);
	}
	return ret;
}

bool Game::RemoveInputHandler(const shared_ptr<IInputHandler>& inputHandler) {
	size_t old_size(inputHandlers.size());
	inputHandlers.erase(remove(inputHandlers.begin(), inputHandlers.end(), inputHandler), inputHandlers.end());
	return old_size != inputHandlers.size();
}

void Game::ClearInputHandlers() noexcept {
	inputHandlers.clear();
}
