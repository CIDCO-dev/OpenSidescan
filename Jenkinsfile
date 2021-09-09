pipeline {

  environment {
    major=0
    minor=1
    patch="${env.BUILD_ID}"
    name="${env.JOB_NAME}"
    version="$major.$minor.$patch"
    exec_name="OpenSidescan-$version"
    publishDir="/var/www/html/$name/$version"
    publishTestOutputWinx64Dir="$publishDir/TestOutputWinx64"
    lastPublishDir="/var/www/html/$name/last"
    binMasterPublishDir="$publishDir/Linux"
    binWinx64Dir="windows-x64"
    binWinx64PublishDir="$publishDir/$binWinx64Dir"
  }
	agent none

  
  
  
  stages {
	/*
    stage('Test file locking on linux'){
      agent { label 'master'}
      steps {
        sh 'Scripts/build_lock_test.sh'
        sh 'mkdir -p build/reports'
        sh 'test/linuxFileLockTest/build/lockTests -r junit -o build/reports/lock-test-report.xml || true'
        sh 'sleep 20' //allow all processes forked in tests to stop
        sh 'Scripts/cutReport.sh' //Cut the second set of test result from the forked process
        junit 'build/reports/cut-report.xml'
      }
    }*/
	/*
    stage('Test file locking on WINDOWS 10') {
        agent { label 'windows10-build-opensidescan-vm'}
        steps {
            
            //compile winlocker
            bat "Scripts/winlocktest.bat"
			bat "echo %cd%"
			bat "echo %cd%"
			bat "test\\win-fileLock-test\\build\\Debug\\wincatchLockTest.exe -r junit -o build\\reports\\winlock-test-report.xml"
        }
        post {
            always {
                junit 'build\\reports\\winlock-test-report.xml'

            }
        }
    }*/
	/*
    stage('Unit tests on linux'){
      agent { label 'master'}
      steps {
        sh 'Scripts/build_linux_unit_tests.sh'
        sh 'mkdir -p build/reports'
        sh 'test/build/tests -r junit -o build/reports/opensidescan-linux-test-report.xml || true'
        junit 'build/reports/opensidescan-linux-test-report.xml'
      }
    }
	
    stage('Unit tests WINDOWS 10') {
        agent { label 'windows10-build-opensidescan-vm'}
        steps {
            bat "Scripts/win-unittest.bat"
			bat "test\\build\\tests.exe -r junit -o build\\reports\\win-unittest.xml"
        }
        post {
            always {
                junit 'build\\reports\\win-unittest.xml'
            }
        }
    }
	
    stage('Build opensidescan linux'){
      agent { label 'master'}
      steps {
        sh 'Scripts/build_opensidescan.sh'
      }
    }
	
    stage('BUILD OPENSIDESCAN FOR WINDOWS 10'){
      agent { label 'windows10-build-opensidescan-vm'}
      steps {
		bat "Scripts/build_opensidescan_win.bat"
		stash includes: 'build/Release/**' , name: 'executable'
      }
    }
	
    stage('SIGN EXECUTABLE WINDOWS 10'){
      agent{label 'windows10-x64-2'}
      steps{
      	unstash 'executable'
        bat "Scripts/sign_exe.au3"
        stash includes: 'build/Release/**' , name: 'executable'
       }
     }
    stage('PACKAGE INSTALLER FOR WINDOWS 10'){
      agent { label 'windows10-build-opensidescan-vm'}
      steps {
      	unstash 'executable'
		bat "Scripts/build_installer.bat"
		stash includes: 'build/**' , name: 'installer'
      }
    }
	
    //todo : passer version en argument
	
    stage('SIGN INSTALLER WINDOWS 10'){
      agent{label 'windows10-x64-2'}
      steps{
      	unstash 'installer'
        bat "Scripts/sign_installer.au3"
        archiveArtifacts('build/Opensidescan-1.0.0-win64.exe')

       }
     }
    stage('PUBLISH ON SERVER'){
      agent { label 'master'}
      options {skipDefaultCheckout()}
      steps {
        sh 'mkdir -p $binWinx64PublishDir'
        sh 'cp /var/lib/jenkins/jobs/$name/builds/$patch/archive/build/Opensidescan-1.0.0-win64.exe $binWinx64PublishDir/Opensidescan-1.0.0-win64.exe'
      }
    }
	*/
    stage('Windows GUI tests'){
      agent { label 'windows10-build-opensidescan-vm'}
      steps {
        bat "echo %cd%"
		bat "ScriptsTestGUI/build_test_gui.bat"
		bat "test\\testGUI\\build\\Release\\Opensidescan_gui_Tests.exe -o build\\reports\\testGUI.xml -xunitxml"
		archiveArtifacts('build\\reports\\testGUI.xml')
      }
      post {
        always {
          junit 'build\\reports\\testGUI.xml'
        }
      }
    }
	/*
    stage('PUBLISH WINDOWS TEST RESULTS ON SERVER'){
      agent { label 'master'}
      steps {

        sh 'mkdir -p $publishTestOutputWinx64Dir'

        sh 'ls -al /var/lib/jenkins/jobs/$name/builds/$patch/'
        sh 'ls -al /var/lib/jenkins/jobs/$name/builds/$patch/archive/'

        sh 'cp /var/lib/jenkins/jobs/$name/builds/$patch/archive/build/Debug/testGUI_result_XUNIT.xml $publishTestOutputWinx64Dir'

      }
    }
	*/
  }
  
}
