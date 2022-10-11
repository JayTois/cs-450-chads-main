# Multi-Programming Executive (MPX)

## Getting Started

1. Download and install [VirtualBox](https://www.virtualbox.org/wiki/Downloads)
2. Download [LOUD](http://loud.csee.wvu.edu/ova/)
3. Import LOUD into VirtualBox
4. Start LOUD
5. Sign in using the password `loud`
6. Open the Terminal
7. Connect to GitHub Using an SSH Key

    1. Generate an SSH Key
    
        > Enter `ssh-keygen -t ed25519 -C "your_email@example.com"`
        
    2. Copy the SSH Key
    
        > Enter `cat /home/loud/.ssh/id_ed25519.pub`
        
        Highlight the returned SSH Key and hold Ctrl+Shift+C to copy
        
    3. Add the SSH Key to GitHub
    
8. Sign Commits Using a GPG Key (Optional)

    1. Generate a GPG Key
    
        > Enter `gpg --full-generate-key`
        
    2. Copy the GPG Key ID
    
        > Enter `gpg --list-secret-keys --keyid-format=long`
        
        Highlight the returned GPG Key ID and hold Ctrl+Shift+C to copy
        
    3. Copy the GPG Key
    
        > Enter `gpg --armor --export <GPG_Key_ID>`
        
        Highlight the returned GPG Key and hold Ctrl+Shift+C to copy
        
    4. Add the GPG Key to GitHub

## User's Manual

Found under the [Wiki](https://github.com/koreykeefe/cs-450-chads/wiki) tab

## Programmer's Manual

Found under the [Wiki](https://github.com/koreykeefe/cs-450-chads/wiki) tab

## Who Did What?

Found under the [Wiki](https://github.com/koreykeefe/cs-450-chads/wiki) tab
