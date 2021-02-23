#include "plugin.hpp"

#include <cmath>

#include "DSTracker.hpp"


struct Sinusoid : Module {
	enum ParamIds {
		THRESHOLD_PARAM,
		MAXFREQ_PARAM,
		MINFREQ_PARAM,
		STICKY_PARAM,
		GAIN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		SIGNAL_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AMP_OUTPUT,
		VOCT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Sinusoid() :
		lastMinFreq(300.0f),
		lastMaxFreq(3000.0f)
	{

		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(THRESHOLD_PARAM, 0.f, 5.f, 0.1f, "Threshold", "v");
		configParam(MAXFREQ_PARAM, 100.f, 22100.f, lastMaxFreq, "Maximum Frequency", "Hz");
		configParam(MINFREQ_PARAM, 100.f, 22100.f, lastMinFreq, "Minimum Frequency", "Hz");
		configParam(STICKY_PARAM, 0.f, 1.f, 0.f, "Sticky Frequency");
		configParam(GAIN_PARAM, 0.f, 5.f, 1.f, "Gain", "v");

		dst = nullptr;

		onSampleRateChange();

	}

	void process(const ProcessArgs& args) override
	{

		
		float newMinFreq = params[MINFREQ_PARAM].getValue();
		float newMaxFreq = params[MAXFREQ_PARAM].getValue();

		if((lastMinFreq != newMinFreq) or (lastMaxFreq != newMaxFreq))
		{

			lastMinFreq = std::min(newMaxFreq, newMinFreq);
			lastMaxFreq = std::max(newMaxFreq, newMinFreq);

			dst->setFreqRange(lastMinFreq, lastMaxFreq);

		}

		dst->processSample(inputs[SIGNAL_INPUT].getVoltage());

		float ampOut = (dst->resMag * params[GAIN_PARAM].getValue()) - params[THRESHOLD_PARAM].getValue();
		outputs[AMP_OUTPUT].setVoltage(clamp(ampOut, 0.f, 10.f));

		if((ampOut > 0.0f) or (params[STICKY_PARAM].getValue() == 0.f))
		{

			lastVOct = std::log((sRate * dst->resArg) / dsp::FREQ_C4) / std::log(2);

		}
		
		outputs[VOCT_OUTPUT].setVoltage(lastVOct);
		

	}

	void onSampleRateChange() override
	{

		
		sRate = APP->engine->getSampleRate();
		std::cout << sRate << std::endl;
		if(dst != nullptr)
		{

			delete dst;

		}
		dst = new DSTracker(100.f, lastMaxFreq, 8, 2, sRate);
		dst->setFreqRange(lastMinFreq, lastMaxFreq);
		

	}

	float		sRate;
	float		lastVOct;
	float		lastMinFreq;
	float		lastMaxFreq;

	DSTracker*	dst;

};


struct SinusoidWidget : ModuleWidget {
	SinusoidWidget(Sinusoid* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Sinusoid.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<BefacoTinyKnob>(mm2px(Vec(30.75, 26.75)), module, Sinusoid::THRESHOLD_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(13.8, 57.25)), module, Sinusoid::MAXFREQ_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(31.8, 85.22)), module, Sinusoid::MINFREQ_PARAM));
		addParam(createParamCentered<NKK>(mm2px(Vec(8.822, 90.5)), module, Sinusoid::STICKY_PARAM));
		addParam(createParamCentered<BefacoTinyKnob>(mm2px(Vec(8.75, 112.0)), module, Sinusoid::GAIN_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.75, 26.75)), module, Sinusoid::SIGNAL_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.75, 114.507)), module, Sinusoid::AMP_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.75, 114.507)), module, Sinusoid::VOCT_OUTPUT));

	}
};


Model* modelSinusoid = createModel<Sinusoid, SinusoidWidget>("Sinusoid");
