<template>
  <div id="app">

    <div class="container mt-2">

      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          <h1>Camera Slider</h1>
        </div>
      </div>

      <div class="row justify-content-center my-5">
        <div class="container">
          <div class="row justify-content-center">
            <div class="col-8 col-md-9 col-lg-10">
                <div class="form-group">
                  <label for="fname">Feed Rate:</label><br>
                  <input type="range" min="0" max="2000" class="form-control" placeholder="1000" v-model="sendInputFeedRate" v-on:keyup.enter="startSlide" :disabled="!ws.connected">
                  <input type="text" class="form-control" placeholder="Type here" v-model="sendInputFeedRate" v-on:keyup.enter="startSlide" :disabled="!ws.connected">
                </div>
            </div>
          </div>
        </div>
      </div>

      <div class="row justify-content-center my-5">
        <div class="container">
          <div class="row justify-content-center">
            <div class="col-8 col-md-9 col-lg-10">
                <div class="form-group">
                  <label for="fname">Distance:</label><br>
                  <input type="range" min="0" max="1000" class="form-control" placeholder="1000" v-model="sendInputDistance" v-on:keyup.enter="startSlide" :disabled="!ws.connected">
                  <input type="text" class="form-control" placeholder="Type here" v-model="sendInputDistance" v-on:keyup.enter="startSlide" :disabled="!ws.connected">
                </div>
            </div>
          </div>
        </div>
      </div>

      <div class="row justify-content-center my-5">
        <div class="container">
          <div class="row justify-content-center">
            <div class="col-8 col-md-9 col-lg-10">
                <div class="form-group">
                  <label for="fname">Rotate:</label><br>
                  <input type="range" min="0" max="360" class="form-control" placeholder="1000" v-model="sendInputRotate" v-on:keyup.enter="startSlide" :disabled="!ws.connected">
                  <input type="text" class="form-control" placeholder="Type here" v-model="sendInputRotate" v-on:keyup.enter="startSlide" :disabled="!ws.connected">
                </div>
            </div>
          </div>
        </div>
      </div>

      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          <button class="btn btn-primary" @click="startSlide" :disabled="!ws.connected">Start Slider</button>
        </div>
      </div>

      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          <button class="btn btn-primary" @click="pauseSlider" :disabled="!ws.connected">Pause</button>
          <button class="btn btn-primary" @click="resumeSlider" :disabled="!ws.connected">Continue</button>
        </div>
      </div>
      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          <button class="btn btn-primary" @click="cancelSlider" :disabled="!ws.connected">Cancel/Abort</button>
          <button class="btn btn-primary" @click="unlockSlider" :disabled="!ws.connected">Unlock</button>
        </div>
      </div>
      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          <button class="btn btn-primary" @click="relPOS" :disabled="!ws.connected">Relative Positioning</button>
          <button class="btn btn-primary" @click="absPOS" :disabled="!ws.connected">Absolute Positioning</button>
        </div>
      </div>

      <div class="row justify-content-center">
        <div class="col-11 col-md-9">
          <div class="card shadow-lg">
            <div class="card-body">
              <div class="container">
                <div class="row justify-content-center">
                  <div class="col-8 col-md-9 col-lg-10">
                      <div class="form-group">
                        <input type="text" class="form-control" placeholder="Type here" v-model="sendInput" v-on:keyup.enter="sendData" :disabled="!ws.connected">
                      </div>
                  </div>
                  <div class="col-4 col-md-3 col-lg-2">
                    <button class="btn btn-primary" @click="sendData" :disabled="!ws.connected">Send</button>
                  </div>
                </div>
              </div>
              <div class="container">
                <div class="row justify-content-center">
                  <div class="col-12">
                    <div class="form-group mt-3">
                      <textarea id="console" class="form-control" rows="8" v-model="serialBuffer" readonly="readonly"></textarea>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          Powered By <a target="_blank" href="https://github.com/ayushsharma82/WebSerial">WebSerial</a>
        </div>
      </div>
    </div>
  </div>

</template>

<script>
import Socket from './socket';

export default {
  name: 'webserial',

  data(){
    return{
      ws:{
        connected: false,
        //lastPing: new Date()
      },
      serialBuffer: "",
      sendInput: "",
      sendInputFeedRate: 500,
      sendInputDistance: 1000,
      sendInputRotate: 90,
      
    }
  },

  methods:{
    sendData(){
      if(this.sendInput.trim() != ""){
        Socket.send(this.sendInput.trim());
        this.sendInput = "";
      }
    },
    startSlide(){
      var cmd = "G1 X"+this.sendInputDistance+" Y"+this.sendInputRotate+" F"+this.sendInputFeedRate;
      Socket.send(cmd);
    },
    homeSlider(){
      Socket.send("$H");
    },
    cancelSlider(){
      Socket.send(String.fromCharCode(24));//sends ctrl-x 
    },
    pauseSlider(){
      Socket.send("!");
    },
    resumeSlider(){
      Socket.send("~");
    },
    unlockSlider(){
      Socket.send("$X");
    },
    relPOS(){
      Socket.send("G91");
    },
    absPOS(){
      Socket.send("G90");
    }
  },

  mounted(){
    Socket.$on("connected", () => {
      this.ws.connected = true;
      Socket.send("?");
    });

    Socket.$on("disconnected", () => {
      this.ws.connected = false;
    });

    Socket.$on("message", (msg) => {
      this.ws.connected = true;
      this.serialBuffer = this.serialBuffer + msg;
      let textarea = this.$el.querySelector("#console");
      textarea.scrollTop = textarea.scrollHeight;
    });
  }
}
</script>

<style lang="scss">
.logo{
  user-select: none;
  -moz-user-select: none;
  -webkit-user-drag: none;
  -webkit-user-select: none;
  -ms-user-select: none;
}

.logo.inactive{
  filter: grayscale(100%);
}

textarea[readonly] {
     cursor: text !important;
}

</style>
