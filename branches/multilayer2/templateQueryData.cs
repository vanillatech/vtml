using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin
{
    class templateQueryData
    {
        public bool learnmode;
        public int outputLMT;
        public int desiredOutput;
        public int[] input;
        public int[] context;
        public string outputFeature;
        public string[] inputFeature;
        public string[] featureMatrix;
        public string token;
        public double activationThreshold;
        public int temporalPatternLength;
        public int maxlayer;
        public double leakageFactor;
        public bool activateNewNeurons;
        public bool temporary;
        /*
        public Result[] results;

        public class Result
        {
            public String message_id;
            public String registration_id;
            public String error;
        };*/
    }
}
