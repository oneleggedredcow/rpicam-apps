#include <chrono>

#include <libcamera/stream.h>
#include <zmq.hpp>

#include "core/rpicam_app.hpp"

#include "post_processing_stages/post_processing_stage.hpp"

using Stream = libcamera::Stream;

class NetworkPreviewStage : public PostProcessingStage
{
public:
	NetworkPreviewStage(RPiCamApp *app) : PostProcessingStage(app) {}

	char const *Name() const override;

	void Read(boost::property_tree::ptree const &params) override;

	void Configure() override;

	bool Process(CompletedRequestPtr &completed_request) override;

	void Teardown() override;

private:
	Stream *stream_;

	std::string connection_;
	double time_delay_;
	zmq::context_t ctx_;
	std::unique_ptr<zmq::socket_t> sock_;
	std::chrono::steady_clock::time_point last_timestamp_;
};

#define NAME "network_preview"

char const *NetworkPreviewStage::Name() const
{
	return NAME;
}

void NetworkPreviewStage::Read(boost::property_tree::ptree const &params)
{
	connection_ = params.get<std::string>("connection");
	time_delay_ = 1.0 / params.get<double>("fps");
}

void NetworkPreviewStage::Configure()
{
	stream_ = app_->GetMainStream();

    sock_ = std::make_unique<zmq::socket_t>(ctx_, zmq::socket_type::push);
    sock_->connect(connection_);

	last_timestamp_ = std::chrono::steady_clock::now();
}

bool NetworkPreviewStage::Process(CompletedRequestPtr &completed_request)
{
	BufferWriteSync w(app_, completed_request->buffers[stream_]);
	libcamera::Span<uint8_t> buffer = w.Get()[0];

	auto timestamp = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp - last_timestamp_);
	if (elapsed.count() >= time_delay_)
	{
    	sock_->send(zmq::buffer(buffer.data(), buffer.size()), zmq::send_flags::dontwait);
		last_timestamp_ = timestamp;
	}

	return false;
}

void NetworkPreviewStage::Teardown()
{
	sock_->close();
	ctx_.close();
}

static PostProcessingStage *Create(RPiCamApp *app)
{
	return new NetworkPreviewStage(app);
}

static RegisterStage reg(NAME, &Create);
