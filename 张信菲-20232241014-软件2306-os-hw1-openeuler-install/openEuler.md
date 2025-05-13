### **安装经验分享：在 VirtualBox 虚拟机上搭建 openEuler 并安装桌面环境**

#### **1. 安装 VirtualBox**

- **下载地址**：前往 [VirtualBox 官方下载页面](https://www.virtualbox.org/wiki/Downloads)。
- **注意事项**：
  - 确保下载与你的操作系统版本兼容的 VirtualBox。
  - 安装过程中，可能会提示安装增强功能或更新，建议按照提示完成，以确保虚拟机性能更优。

---

#### **2. 搭建 openEuler 并安装桌面环境**

##### **2.1 下载镜像**

- **推荐版本**：openEuler 20.03 LTS 版本，该版本相对稳定。
- **下载地址**：[openEuler 20.03 LTS 镜像下载](https://repo.openeuler.org/openEuler-20.03-LTS/ISO/x86_64/)。
- **注意事项**：
  - 确认下载的镜像文件完整无误，可通过校验文件的 MD5 或 SHA256 值来验证。
  - 如果需要更高版本（如 openEuler 22.09），需注意其在虚拟机上的兼容性可能不如 20.03 LTS。

---

##### **2.2 新建虚拟机**

1. 打开 VirtualBox，点击“新建虚拟机”。
2. **设置虚拟机参数**：
   - **名称**：随意命名，例如“openEuler-20.03”。
   - **操作系统类型**：选择 Linux，版本选择 openEuler。
   - **内存大小**：建议至少分配 2048MB，如果系统运行缓慢，可适当增加。
   - **处理器数量**：根据主机性能分配，建议至少 2 个 CPU。
   - **虚拟硬盘**：选择默认的动态分配，大小建议 20GB 或以上。
   - **网络适配器**：选择“桥接网卡”，连接到你的网络。
  ！[设置虚拟机参数](screen.png)
1. **注意事项**：
   - 在设置虚拟机时，确保镜像文件路径正确。
   - 如果主机内存较小，分配过多内存可能会导致主机卡顿，需根据实际情况调整。

---

##### **2.3 连网**

1. 打开虚拟机设置，选择“网络”。
2. **网络模式**：
   - 推荐选择“桥接网卡”，这样虚拟机可以像独立主机一样直接访问外部网络。
   - 如果选择“NAT”模式，可能会出现网络不稳定的情况。
  ！[网络模式](net.png)
1. **注意事项**：
   - 如果网络连接失败，可能是网卡未正确配置。可以尝试重启虚拟机或重新设置网络模式。
   - 如果出现“Fail to establish a new connection”错误，通常是因为网络连接不稳定，多尝试几次即可。

---

##### **2.4 启动虚拟机并登录**

1. 启动虚拟机后，等待系统加载完成。
   ！[启动虚拟机](start.png)
2. **登录方式**：
   - 使用 `root` 用户登录，密码为创建虚拟机时设置的密码"changeme"。
   - 如果之前设置了其他用户，也可以尝试登录，但后续配置可能需要 `root` 权限。
3. **注意事项**：
   - 如果虚拟机启动后提示“Fail to establish a new connection”，不要慌，等待一段时间可能会自动连接。
   - 如果长时间无法连接，可以尝试重启虚拟机或检查网络设置。

* ---

##### **2.5 配置源**

1. **配置清华源**（推荐）：
   - 打开终端，输入以下命令：
     ```bash
     sudo vi /etc/yum.repos.d/openEuler_x86_64.repo
     ```
   - 如果 `vi` 编辑器不熟悉，可以使用 `nano`：
     ```bash
     sudo nano /etc/yum.repos.d/openEuler_x86_64.repo
     ```
   - 在文件末尾添加以下内容：
     ```
     [osrepo]
     name=osrepo
     baseurl=https://mirrors.tuna.tsinghua.edu.cn/openeuler/openEuler-20.03-LTS/OS/x86_64/
     enabled=1
     gpgcheck=1
     gpgkey=https://mirrors.tuna.tsinghua.edu.cn/openeuler/openEuler-20.03-LTS/OS/x86_64/RPM-GPG-KEY-openEuler
     ```
   - 保存并退出编辑器。
2. **注意事项**：
   - 配置源时，确保文件格式正确，否则可能导致后续安装失败。
   - 如果使用官方源，下载速度可能较慢，不推荐。

---

#### **3. 安装 GNOME 桌面**

1. **安装桌面环境**：
   - 在终端中依次输入以下命令：
     ```bash
     dnf install gnome-shell gdm gnome-session
     dnf install gnome-terminal
     ```
   - 这些命令会安装 GNOME 桌面环境及相关组件。
2. **设置开机自启动**：
   - 输入以下命令：
     ```bash
     systemctl enable gdm.service
     systemctl set-default graphical.target
     ```
3. **补全丢失文件**（可选）：
   - 如果在安装过程中遇到问题，可以尝试以下命令：
     ```bash
     cd /tmp
     wget https://gitee.com/name1e5s/xsession/raw/master/Xsession
     mv Xsession /etc/gdm/
     chmod 0777 /etc/gdm/Xsession
     ```
4. **注意事项**：
   - 安装过程中可能会提示需要下载大量文件，耐心等待即可。
   - 如果安装完成后无法进入桌面环境，可以尝试重启虚拟机。

---

#### **4. 安装火狐浏览器**

1. **安装火狐**：
   - 在终端中输入以下命令：
     ```bash
     yum -y install firefox
     ```
   - 这将自动安装火狐浏览器。
2. **注意事项**：
   - 如果安装过程中提示权限不足，确保以 `root` 用户登录。
   - 安装完成后，可以在桌面或应用菜单中找到火狐浏览器图标。

---

### **注意事项**

1. **镜像完整性验证**：
   - 下载完成后，使用完整性检查文件（如 SHASUM）验证 ISO 文件的完整性，以确保下载的镜像文件未损坏。

2. **网络连接问题**：
   - 如果虚拟机无法联网，可能是网络模式未正确配置，建议使用“桥接网卡”。
   - 如果网络连接不稳定，可以尝试重启虚拟机或重新设置网络。

3. **用户权限问题**：
   - 安装桌面环境和浏览器时，需要 `root` 权限或使用 `sudo` 命令。

4. **磁盘空间不足**：
   - 安装桌面环境可能会占用大量磁盘空间，建议虚拟硬盘大小设置为 20GB 或以上。

5. **安装后更新**：
   - 安装完成后，建议立即更新系统，以获取最新的软件包和安全补丁。

6. **安装失败的排查**：
   - 如果安装过程中遇到问题，可以尝试进入“Troubleshooting”模式进行问题定位。
   - 也可以尝试重新安装，或在 VirtualBox 上安装其他发行版，以排查问题。
