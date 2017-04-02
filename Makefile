vbox:
	-vagrant destroy
	vagrant box update
	vagrant up --provider=virtualbox
vtt:
	vagrant reload
	vagrant ssh
vps:
	VBoxManage list runningvms
vpackage:
	vagrant package --output bus1.box
