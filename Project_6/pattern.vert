#version 120

uniform float    uTime;        // "Time", from Animate( )
uniform int     uBottonControl;
varying vec2      vST;        // texture coords

const float PI =     3.14159265;
const float AMP =     0.2;        // amplitude
const float W =     2.;        // frequency

varying vec3 vN;// normal vector
varying vec3 vL;// vector from point to light
varying vec3 vE; // vector from point to eye

const vec3 LIGHTPOSITION = vec3( 0., 5., 5. );
void
main( )
{
    
    
    vST = gl_MultiTexCoord0.st;
    vec3 vert = gl_Vertex.xyz;
    vec3 norm = normalize( vert );
    
    // vert.x +=  (cos(1.PI*W*uTime-vST)) ;
    //  vert.y += 0.9 * (sin(PI*uTime))+1;
    // vert.z *= 0.9 * (sin(PI*uTime))+1;
    vert += AMP * norm * cos(W*PI*(uTime-vST.s) );

    vST = gl_MultiTexCoord0.st;
    vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
    vec4 p = gl_ModelViewMatrix * vec4 (LIGHTPOSITION,1);
    vec3 uLightPosition = p.xyz;
    vN = normalize( gl_NormalMatrix * gl_Normal );// normal vector
    vL = LIGHTPOSITION - ECposition.xyz;// vector from the point to the light position
    
    vE = vec3(0.0392, 0.051, 0.8824) - ECposition.xyz;// vector from the point to the eye position
    
    gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}


