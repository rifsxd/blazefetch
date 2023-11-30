<div align="center">
	<h1>Blazefetch</h1>
	<blockquote align="center">⚡ A lite & blazing fast system info fetch utility.</blockquote>
	<p><br>
		⚡ This is blazefetch a simple and useful tool to get live fetch info through simple flags which will be very helpful for eww widgets and sys bar like polybars, waybars and such, it doesnt need any refresh or anything just keep the -g command in loop, run it at start with --d or daemon and you are good to go, its very minimal and lite which only takes 200-300 kb and 0.03% cpu every 1 sec (also taking suggestions on which other info should it fetch in the future) !!!THIS IS NOT YOUR AVERAGE FETCH SCRIPT!!!
	</p><br>
	<p><br> Default</p><br>
	<img src="/assets/default_prev.png">
	<p><br> Iconic</p><br>
	<img src="/assets/iconic_prev.png">
	<p><br> Legacy</p><br>
	<img src="/assets/legacy_prev.png">
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
	blazefetch [OPTIONS]\n"
    	Options:
    	  -d, --daemon      Run as a daemon (required)
		  -g, --get <INFO>  Get and display specific information (e.g., OS, GPU)
          -c, --clear       Clears the stored cache from memory
          -v, --version     Show version information
          -h, --help        Show this help message

# License
Blazefetch is licensed under the MIT license.  
[Read here](LICENSE) for more info.
