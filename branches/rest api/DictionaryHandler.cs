using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using odin.model;

namespace odin
{ 
    public class DictionaryHandler 
    {
        
        private Dictionary<String, Brain> brains = new Dictionary<string, Brain>();

        private DictionaryHandler()
        {
            Console.WriteLine("Initializing singleton object");

            
        }

        private static Lazy<DictionaryHandler> instance = new Lazy<DictionaryHandler>(() => new DictionaryHandler());

        public static DictionaryHandler Instance = instance.Value;
        public Dictionary<String, Brain> getBrainDictionary()
        {
            return brains;
        }
    }
}


