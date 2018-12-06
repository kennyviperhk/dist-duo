precision highp float; // this will make the default precision high

//we passed this in from our vert shader
varying vec2        texcoord0;

//These are variables we set in our ofApp using the ofShader API

//our texture reference
//passed in by
//shader.setUniformTexture("tex0", sourceImage.getTextureReference(), sourceImage.getTextureReference().texData.textureID);
//uniform sampler2D   tex0;
uniform sampler2D tex0;
//width and height that we are working with
//passed in by
//shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
uniform vec2        resolution;

//a changing value to work with
//passed in by
//shader.setUniform1f("time", ofGetElapsedTimef());
uniform float       time;
uniform float       rand;

//Each shader has one main() function you can use
//Below are a few implementations. Make sure you have all but one commented out

//Shaders are compiled at runtime meaning that you can just change the shader file 
//and re-run the ofApp without compiling


// just draw the texture to screen		
/*void main()
{
    gl_FragColor = texture2D(tex0, texcoord0);
}*/


/*
// draw the texture to screen, inverted
void main()
{
    vec4 texColor = texture2D(tex0, texcoord0);  
    gl_FragColor = vec4(texColor.r+rand, 1.0-texColor.g, 1.0-texColor.b, texColor.a);
}*/




// let's wobble the image channels around independently, a bit Fear and Loathing in Las Vegas style
void main()
{
    mediump float newTime = time * 2.0;

    vec2 newTexCoord = texcoord0;
  //  newTexCoord.s = texcoord0.s + (cos(newTime + (texcoord0.s*20.0*rand)) * 0.01);
   // newTexCoord.t = texcoord0.t + (sin(newTime + (texcoord0.t*20.0*rand*rand)) * 0.01);

 //   newTexCoord.s = texcoord0.s + floor((cos(newTime + (texcoord0.s*50.0*rand)) * 0.01));
 //   newTexCoord.t = texcoord0.t + floor(sin(newTexCoord.s/30.0*rand+rand*rand))*10.0*rand;
    
   //    newTexCoord.s = texcoord0.s - floor((cos(newTime + (texcoord0.s*50.0*rand)) * 0.01));
  //  newTexCoord.t =newTexCoord.t+ newTexCoord.t* newTexCoord.s*rand*-0.03;

//	newTexCoord.s =newTexCoord.s+ newTexCoord.t* newTexCoord.s*rand*-0.04;
 //   newTexCoord.t =newTexCoord.t- newTexCoord.t* newTexCoord.s*rand*0.07;
                  newTexCoord.s =  texcoord0.s*texcoord0.s+ (rand-texcoord0.t*(rand-0.5));
                  newTexCoord.t =texcoord0.t+texcoord0.t*texcoord0.s*(rand*rand-0.5);

    
    mediump vec2 texCoordRed    = newTexCoord;
    mediump vec2 texCoordGreen  = newTexCoord;
    mediump vec2 texCoordBlue   = newTexCoord;

  //  texCoordRed     +=  vec2(floor(sin(newTexCoord.t/30.0*rand+rand*rand))*10.0*rand,0);
 //   texCoordGreen   += vec2(floor(sin(newTexCoord.t/30.0*rand+rand*rand))*20.0*rand,0);
  //  texCoordBlue    +=  vec2(floor(sin(newTexCoord.t/30.0*rand+rand*rand))*30.0*rand,0);  
//	vec4 colorRR = texture2DRect( tex0, texCoordRed );


    mediump float colorR = texture2D( tex0, texCoordRed ).r;
    mediump float colorG = texture2D( tex0, texCoordGreen).g;
    mediump float colorB = texture2D( tex0, texCoordBlue).b;  
	mediump float colorA = texture2D( tex0, texCoordBlue).a;     
    mediump vec4 outColor = vec4( colorR, colorG, colorB, colorA);

    gl_FragColor = outColor;
}


/*

void main()
{
  
    vec2 texCoord = vec2(texcoord0.s, texcoord0.t);
    vec4 col = texture2D(tex0,texCoord);
    vec4 col_s = texture2D(tex0,texCoord + vec2(floor(sin(texcoord0.t/30.0*rand+rand*rand))*30.0*rand,0));
    
    col = col_s;

/*
    mediump float colorR = texture2D( tex0, texCoordRed ).r;
    mediump float colorG = texture2D( tex0, texCoordGreen).g;
    mediump float colorB = texture2D( tex0, texCoordBlue).b;  
	mediump float colorA = texture2D( tex0, texCoordBlue).a;     
    mediump vec4 outColor = vec4( colorR, colorG, colorB, colorA);
outColor = col;*/	
  //  gl_FragColor = col;
//}
//*/




/*
//This is the internal RPi vert shader for reference
precision lowp float;

uniform sampler2D src_tex_unit0;

uniform float usingTexture;
uniform float bitmapText;

varying vec4 colorVarying;
varying vec2 texCoordVarying;

void main(){
	vec4 tex;
	if(usingTexture>.5){
		tex = texture2D(src_tex_unit0, texCoordVarying);
		if(bitmapText>.5 && tex.a < 0.5){
			discard;
		}else{
			gl_FragColor = colorVarying*tex;
		}
	}else{
		gl_FragColor = colorVarying;
	}
}
*/
