import sys, os
import unittest
from pathlib import Path

class TestInstallation(unittest.TestCase):
    def testModelExists(self):
        onnxModelDirPath = "C:\Program Files\OpenSidescan\models"

        wmmModelPath = "C:\Program Files\OpenSidescan\src\\thirdParty\\MBES-lib\\src\\thirdParty\\WorldMagneticModel\\WMM2020_Linux\\src\\WMM.COF"

        self.assertTrue(os.path.exists(onnxModelDirPath))
        self.assertTrue(os.path.exists(wmmModelPath))

        file = os.listdir(onnxModelDirPath)
        
        self.assertTrue(file[0].endswith(".onnx"))
        

if __name__ == '__main__':
    unittest.main()