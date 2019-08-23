pipeline {

  environment {
    major=0
    minor=1
    patch="${env.BUILD_ID}"
    name="${env.JOB_NAME}"
    version="$major.$minor.$patch"
    exec_name="OpenSidescan-$version"
    publishDir="/var/www/html/$name/$version"
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
      }
    }

    stage('BUILD WINDOWS 10'){
      agent { label 'windows10-x64-2'}
      steps {
        //compile
        bat "Scripts\\buildopensidescan_gui.bat"

        archiveArtifacts('build\\bin\\OpenSidescan.zip')

      }
    }

    stage('PUBLISH ON SERVER'){
      agent { label 'master'}
      steps {
        sh 'mkdir -p $binMasterPublishDir'
        sh 'mkdir -p $binWinx64PublishDir'
        sh 'cp -r build/bin/OpenSidescan $binMasterPublishDir/$exec_name'
        sh 'cp  /var/lib/jenkins/jobs/$name/builds/$patch/archive/build/bin/OpenSidescan.zip  $binWinx64PublishDir/overlap-$version.zip'
      }
    }
  }

}
