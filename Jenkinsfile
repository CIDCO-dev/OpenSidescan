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
    }

    stage('Test file locking on WINDOWS 10') {
        agent { label 'windows10-x64-2'}
        steps {
            bat "echo %cd%"
            bat "echo %cd%"
            //compile winlocker
            bat "make -f MakefileWindows locktest"
            bat "echo %cd%"
            bat "build\\test\\bin\\winLockTest.exe -r junit -o build\\reports\\winlock-test-report.xml"
        }
        post {
            always {
                junit 'build\\reports\\winlock-test-report.xml'

            }
        }
    }

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
        agent { label 'windows10-x64-2'}
        steps {
            bat "make -f MakefileWindows test"
        }
        post {
            always {
                junit 'build\\reports\\opensidescan-win-test-report.xml'
            }
        }
    }

    stage('Build linux installer'){
      agent { label 'master'}
      steps {
        sh 'make'
        sh 'Scripts/build_installer.sh $version'
        archiveArtifacts('OpenSidescan_installer*.run')
      }
    }

    

    stage('BUILD WINDOWS 10'){
      agent { label 'windows10-x64-2'}
      steps {
        //compile
        bat "Scripts\\build_opensidescan_gui.bat"

        script {
            if ( fileExists('build\\release\\OpenSidescan.exe') == false) {
                echo 'Jenkinsfile: build\\release\\OpenSidescan.exe does not exist, calling error()'
                error("Build failed because 'build\\release\\OpenSidescan.exe' does not exist")
            }
        }


        bat "Scripts\\sign_exe.au3"
        bat "Scripts\\package_opensidescan_gui.bat"
        bat "Scripts\\build_installer.bat %version%"
        bat "Scripts\\sign_installer.au3 %version%"

        archiveArtifacts('OpenSidescan_installer*.exe')


      }
    }

    stage('PUBLISH ON SERVER'){
      agent { label 'master'}
      steps {
        sh 'mkdir -p $binMasterPublishDir'
        sh 'mkdir -p $binWinx64PublishDir'

        sh 'cp /var/lib/jenkins/jobs/$name/builds/$patch/archive/OpenSidescan_installer_$version.run $binMasterPublishDir/OpenSidescan_installer_$version.run'
        sh 'cp /var/lib/jenkins/jobs/$name/builds/$patch/archive/OpenSidescan_installer_$version.exe $binWinx64PublishDir/OpenSidescan_installer_$version.exe'
      }
    }

    stage('BUILD TEST WINDOWS 10 AND RUN TEST'){
      agent { label 'windows10-x64-2'}
      steps {

        bat "echo %cd%"

        bat "ScriptsTestGUI\\build_test_gui.bat"
        bat "ScriptsTestGUI\\copy_dll_for_test_gui_gui.bat"
        bat "ScriptsTestGUI\\run_test_gui.bat"

        bat "echo %cd%"

        archiveArtifacts('buildTest\\release\\folderRunTest\\test-report-OpenSidescanXUNIT.xml')
        archiveArtifacts('buildTest\\release\\folderRunTest\\test-report-OpenSidescan.xml')
        archiveArtifacts('buildTest\\release\\folderRunTest\\test-report-OpenSidescanTAP.txt')
        archiveArtifacts('buildTest\\release\\folderRunTest\\test-report-OpenSidescanTXT.txt')

      }
      post {
        always {
          junit 'buildTest\\release\\folderRunTest\\test-report-OpenSidescanXUNIT.xml'
        }
      }
    }

    stage('PUBLISH WINDOWS TEST RESULTS ON SERVER'){
      agent { label 'master'}
      steps {

        sh 'mkdir -p $publishTestOutputWinx64Dir'

        sh 'ls -al /var/lib/jenkins/jobs/$name/builds/$patch/'
        sh 'ls -al /var/lib/jenkins/jobs/$name/builds/$patch/archive/'

        sh 'cp /var/lib/jenkins/jobs/$name/builds/$patch/archive/buildTest/release/folderRunTest/test-report-OpenSidescan* $publishTestOutputWinx64Dir'

      }
    }
  }
}
