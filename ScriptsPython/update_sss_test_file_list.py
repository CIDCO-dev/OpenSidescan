import sys,os
import pyxtf
import argparse
import paramiko

class SssAggregat0r:
	def __init__(self, host:str, user:str, password:str, location:str, download_path:str):
		self.__host = host
		self.__user = user
		self.__password = password
		self.download_path = download_path
		self.__location = location
		
	def download_files(self, files:list):
		transport = paramiko.Transport((self.__host, 22))
		transport.connect(None, self.__user, self.__password)
		sftp = paramiko.SFTPClient.from_transport(transport)
		
		for f in files:
			f = f[8:] # sftp cannot see the hdd
			download_path = os.path.join(self.download_path, os.path.basename(f))
			sftp.get(f, download_path)
		
		sftp.close()
		transport.close()
		
	def is_mbes(self, filePath):
		try:
			(file_header, packets) = pyxtf.xtf_read(filePath)
			if file_header.NumberOfBathymetryChannels > 0:
				return True
			else:
				False
		except Exception as e:
			sys.stderr.write("exception : {} \n".format(repr(e)))
		
	
	def list_xtf_files_on_remote_server(self):
		client = paramiko.SSHClient()
		client.load_system_host_keys()
		client.connect(self.__host, username=self.__user, password=self.__password)
		stdin, stdout, stderr = client.exec_command('bash')
		stdin.write('find {} -iname *.xtf -printf "%P\n"'.format(self.__location))
		stdin.channel.shutdown_write()
		
		files = []
		for line in stdout.readlines():
			files.append(os.path.join(self.__location, line[:-1]))
			
		stdin.close()
		stdout.close()
		stderr.close()
		client.close()
		
		return files
	
	def get_1_file_per_mission(self, files):
		d = dict()
		for f in files:
			path = os.path.dirname(f)
			if path not in d:
				d[path] = f
		
		return d.values()



#========================= MAIN ============================#

parser=argparse.ArgumentParser(description="Cidco 2023 {} get one SSS (xtf) per mission".format(os.linesep))
parser.add_argument("host", type=str, help="ssh host")
parser.add_argument("user", type=str, help="ssh user")
parser.add_argument("password", type=str, help="ssh password")
parser.add_argument("location", type=str, help="path where to start searching for files on server")
parser.add_argument("download_path", type=str, help="local path to store files")
args = parser.parse_args()


aggregator = SssAggregat0r(args.host, args.user, args.password, args.location, args.download_path)

files = aggregator.list_xtf_files_on_remote_server()

oneFilePerMission = aggregator.get_1_file_per_mission(files)

aggregator.download_files(oneFilePerMission)

for f in os.listdir(aggregator.download_path):
	path = os.path.join(aggregator.download_path, f)
	if aggregator.is_mbes(path):
		os.remove(path)

