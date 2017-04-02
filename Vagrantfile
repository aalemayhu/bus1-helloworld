# -*- mode: ruby -*-
# vi: set ft=ruby :
Vagrant.configure("2") do |config|
  config.vm.box = "fedora/25-cloud-base"
  config.vm.synced_folder ".", "/vagrant", disabled: true
  config.vm.provision "shell", run: 'always', inline: <<-SHELL
     dnf update -y
     dnf install -y git kernel-devel-`uname -r` gcc
     git clone https://github.com/bus1/bus1 /tmp/bus1
     setenforce 0
     make -C /tmp/bus1 module
     export KERNELVER=`uname -r`
     mkdir -p /lib/modules/$KERNELVER/kernel/ipc/bus1/
     cp -f /tmp/bus1/ipc/bus1/bus1.ko /lib/modules/$KERNELVER/kernel/ipc/bus1/
     depmod $KERNELVER
     make -C /tmp/bus1 tt
  SHELL
end
