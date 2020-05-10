
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Web.Http;

using System.Web.Script.Serialization;
using System.IO;
using odin.model;
using System.Net.Http;

namespace odin
{
    public class QueryController : ApiController
    { 
        [HttpGet]
        public HttpResponseMessage getElement(string query)
        {
            string ret = HandleClientComm(query);
        
            var response = Request.CreateResponse(HttpStatusCode.OK);
            response.Content = new StringContent(ret, Encoding.UTF8, "application/json");
            return response;

        }


        [HttpPost]
        public HttpResponseMessage Post(HttpRequestMessage query)
        {
        
            string data = query.Content.ReadAsStringAsync().Result;
            string ret = HandleClientComm(data);
            var response = Request.CreateResponse(HttpStatusCode.OK);
            response.Content = new StringContent(ret, Encoding.UTF8, "application/json");
            return response;
        }

        private string HandleClientComm(string dataFromClient)
        {
            var db = DictionaryHandler.Instance;
            Dictionary<String, Brain> brains = db.getBrainDictionary();
            string datarec = "";
 
            if (dataFromClient != null)
            {
                if (dataFromClient.IndexOf("\n") > -1)
                {
                    dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("\n"));
                }
                try
                {
                    templateQueryData inp = new JavaScriptSerializer().Deserialize<templateQueryData>(dataFromClient);

                    if (inp.token != null)
                    {
                        if (!brains.ContainsKey(inp.token))
                        {
                            if (File.Exists(inp.token + ".dump"))
                            {
                                try
                                {
                                    FileStream bd = File.OpenRead(inp.token + ".dump");
                                    var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                                    brains.Add(inp.token, (Brain)binaryFormatter.Deserialize(bd));
                                    bd.Close();

                                }
                                catch (Exception ex)
                                {
                                    datarec = "Error: Could not read file from disk. Original error: " + ex.Message;
                                }
                                brains[inp.token].init();
                            }
                            else
                            {
                                brains.Add(inp.token, new Brain());
                            }
                        }
                        if (inp.activationThreshold > 0)
                        {
                            brains[inp.token].activationThreshold = inp.activationThreshold;
                        }
                        if (inp.activateNewNeurons == true)
                        {
                            brains[inp.token].activateNewNeurons = inp.activateNewNeurons;
                        }
                        if (inp.noSeparateFeatures)
                        {
                            brains[inp.token].separateFeatures = false;
                        }
                        if (inp.maxlayer > 0)
                        {
                            brains[inp.token].maxLayer = inp.maxlayer;
                        }
                        if (inp.temporalPatternLength > 0)
                        {
                            brains[inp.token].temporalPatternLength = inp.temporalPatternLength;
                        }
                        if (inp.leakageFactor > 0)
                        {
                            brains[inp.token].leakageFactor = inp.leakageFactor;
                        }
                        if (inp.token.Contains("READONLY"))
                        {
                            inp.learnmode = false;
                        }
                        if (inp.temporary == true)
                        {
                            brains[inp.token].temporary = inp.temporary;
                        }
                        //* translate features to int representation
                        if (inp.featureMatrix != null)
                        {
                            inp.context = new int[inp.featureMatrix.Count()];
                            for (int n = 0; n < inp.featureMatrix.Count(); n++)
                            {
                                inp.context[n] = brains[inp.token].getFeatureAsInt(inp.featureMatrix[n]);
                            }
                        }
                        if (inp.inputFeature != null)
                        {
                            inp.input = new int[inp.inputFeature.Count()];
                            for (int n = 0; n < inp.inputFeature.Count(); n++)
                            {
                                inp.input[n] = brains[inp.token].getFeatureAsInt(inp.inputFeature[n]);
                            }
                        }
                        if (inp.outputFeature != null)
                        {
                            inp.desiredOutput = brains[inp.token].getFeatureAsInt(inp.outputFeature);

                        }
                        //*
                        if (inp.desiredOutput != 0)
                        {
                            brains[inp.token].desiredOutput = inp.desiredOutput;
                        }
                        else if (inp.input != null)
                        {
                            brains[inp.token].desiredOutput = inp.input[0];
                        }
                        else
                        {
                            brains[inp.token].desiredOutput = 0;
                        }
                        datarec = brains[inp.token].query(inp.input, inp.context, inp.learnmode);
                        brains[inp.token].think(inp.outputLMT);
                        if (!inp.learnmode)
                        {
                            datarec = "{\"result\": [" + brains[inp.token].getOutput() + "]}";
                        }
                        else
                        {
                            datarec = "Done";
                        }
                           
                    }
                    else
                    {
                        datarec = "{\"error\":\"token required.\"}";
                    }
                }
                catch (Exception e)
                {
                    datarec = "{\"error\":\"Input Format error.\"}";
                } 
            }
            else
            {
                return "{\"error\":\"Please provide an input!\"}";
            }

            return datarec;
        }
    }
}
