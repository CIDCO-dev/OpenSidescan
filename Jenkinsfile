pipeline {

  environment {
    major=0
    minor=1
    patch="${env.BUILD_ID}"
    name="${env.JOB_NAME}"
    version="$major.$minor.$patch"
    exec_name="OpenSidescan-$version"
    publishDir="/var/www/html/$name/$version"
    publishTestOutputLinuxDir="$publishDir/TestOutputLinux"
    lastPublishDir="/var/www/html/$name/last"
    binMasterPublishDir="$publishDir/Linux"
    binWinx64Dir="windows-x64"
    binWinx64PublishDir="$publishDir/$binWinx64Dir"
  }

  agent none
  stages {

    stage('BUILD MASTER'){
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

    stage('TEST MASTER'){
      agent { label 'master'}
      steps {
        sh 'make testGUI'
      }
      post {
        always {
          sh 'mkdir -p $publishTestOutputLinuxDir'
          sh 'cp build/test/bin/test-report-OpenSidescan* $publishTestOutputLinuxDir/'
        }
      }

    }


  }

}
