<div align="center">
	<h1>Blazefetch</h1>
	<blockquote align="center">⚡ A lite & blazing fast system info fetch utility.</blockquote>
	<p><br>
		⚡ This is blazefetch a simple and useful tool to get live fetch info through simple flags which will be very helpful for eww widgets and sys bar like polybars, waybars and such, it doesnt need any refresh or anything just keep the -g command in loop, run it at start with --d or daemon and you are good to go, its very minimal and lite which only takes 200-300 kb and 0.03% cpu every 1 sec (also taking suggestions on which other info should it fetch in the future) !!!THIS IS NOT YOUR AVERAGE FETCH SCRIPT!!!
	</p><br>
        </p><br>
	[![GitHub license](https://img.shields.io/github/license/rifsxd/blazefetch?logo=apache&label=License&style=flat)](https://github.com/rifsxd/blazefetch/blob/master/LICENSE)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/rifsxd/blazefetch/total?logo=github&label=Downloads&style=flat)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rifsxd/blazefetch?style=flat&label=Code%20Size)
[![GitHub Beta CI Status](https://img.shields.io/github/actions/workflow/status/rifsxd/blazefetch/build.yml?logo=github&label=Beta%20CI&style=flat)](https://github.com/rifsxd/blazefetch/actions/workflows/build.yml)
        </p><br>
	<p><br> Default</p><br>
	<img src="/assets/preview_default.png">
	<p><br> Default With ASCII Art</p><br>
	<img src="/assets/preview_default_ascii.png">
	<p><br> Iconic</p><br>
	<img src="/assets/preview_iconic.png">
	<p><br> Iconic With ASCII Art</p><br>
	<img src="/assets/preview_iconic_ascii.png">
</div>

# Install
Binaries are provided at the releases page [here](https://github.com/rifsxd/blazefetch/releases).

## Manual Compile
```sh
git clone https://github.com/rifsxd/blazefetch
cd blazefetch/src
sudo make clean install
```
 - For just icons as headers
```sh
git clone https://github.com/rifsxd/blazefetch
cd blazefetch/src
sudo make clean install_iconic
```  

# Usage
	blazefetch [OPTIONS]

		Options:
			-d, --daemon                 Run as a daemon
			-g, --get <MODULE>           Get and display specific information (example: OS, GPU, etc)
			-l, --live                   Launch live fetch instance
			-s, --static                 Show static fetch information
			-m, --minimal                Show Minimal fetch information
			-a, --ascii                  Show ascii art of your distro
			-c, --clear                  Clears the stored cache from memory
			-r, --remove                 Removes the lock file incase if something went wrong
			-v, --version                Show version information
			-k, --kill                   Kill the daemon process
			-q, --query <NUMBER>         Query git logs and commits
			-h, --help                   Show this help message

			Combined Options:
			-a -m, --ascii --minimal     Show distro ascii art combined with minimal fetch info
			-a -s, --ascii --static      Show distro ascii art combined with static fetch info

			Extra Options:
			-b, --bunny                  Show cute bunny fetch info
			-p, --pussy                  Show cute pussy fetch info
		
		Env Variables:
			BLZ_OS="Xxxx"                Env variable that OS and GLYPH modules look for custom user set distro names

# License
Blazefetch is licensed under the MIT license.  
[Read here](LICENSE) for more info.
