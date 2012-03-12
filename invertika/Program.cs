using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Drawing;
using System.Threading;
using OpenTK;
using OpenTK.Graphics.OpenGL;
using OpenTK.Input;
using Gwen.Control;

namespace invertika
{
	public class Program : GameWindow
	{
		//static readonly Font TextFont=new Font(FontFamily.GenericSansSerif, 11);
		//Bitmap TextBitmap=new Bitmap(512, 512);
		//int texture;
		//bool mouse_in_window=false;
		//bool viewport_changed=true;
		//bool refresh_text=true;
		//MouseState mouse, mouse_old;
		//KeyboardState keyboard, keyboard_old;

		//public Program()
		//    : base(800, 600)
		//{
		//    VSync=VSyncMode.On;
		//    Keyboard.KeyRepeat=true;
		//    Keyboard.KeyDown+=KeyDownHandler;

		//    MouseEnter+=delegate { mouse_in_window=true; };
		//    MouseLeave+=delegate { mouse_in_window=false; };

		//    Move+=delegate { refresh_text=true; };
		//    Resize+=delegate { refresh_text=true; };
		//    WindowBorderChanged+=delegate { refresh_text=true; };
		//    WindowStateChanged+=delegate { refresh_text=true; };
		//    FocusedChanged+=delegate { refresh_text=true; };
		//    Mouse.Move+=MouseMoveHandler;
		//    Mouse.ButtonDown+=MouseButtonHandler;
		//    Mouse.ButtonUp+=MouseButtonHandler;
		//}

		//void KeyDownHandler(object sender, KeyboardKeyEventArgs e)
		//{
		//    switch(e.Key)
		//    {
		//        case OpenTK.Input.Key.Escape:
		//            if(!CursorVisible)
		//                CursorVisible=true;
		//            else
		//                Exit();
		//            break;

		//        case Key.Number1: WindowState=WindowState.Normal; break;
		//        case Key.Number2: WindowState=WindowState.Maximized; break;
		//        case Key.Number3: WindowState=WindowState.Fullscreen; break;
		//        case Key.Number4: WindowState=WindowState.Minimized; break;

		//        case Key.Number5: WindowBorder=WindowBorder.Resizable; break;
		//        case Key.Number6: WindowBorder=WindowBorder.Fixed; break;
		//        case Key.Number7: WindowBorder=WindowBorder.Hidden; break;

		//        case Key.Left: X=X-16; break;
		//        case Key.Right: X=X+16; break;
		//        case Key.Up: Y=Y-16; break;
		//        case Key.Down: Y=Y+16; break;

		//        case Key.KeypadPlus:
		//        case Key.Plus: Size+=new Size(16, 16); break;

		//        case Key.KeypadMinus:
		//        case Key.Minus: Size-=new Size(16, 16); break;
		//    }
		//}

		//void MouseMoveHandler(object sender, MouseMoveEventArgs e)
		//{
		//    refresh_text=true;
		//}

		//void MouseButtonHandler(object sender, MouseButtonEventArgs e)
		//{
		//    refresh_text=true;

		//    if(e.Button==MouseButton.Left&&e.IsPressed)
		//    {
		//        CursorVisible=false;
		//    }
		//}

		//static int Clamp(int val, int min, int max)
		//{
		//    return val>max?max:val<min?min:val;
		//}

		//static void DrawString(Graphics gfx, string str, int line)
		//{
		//    gfx.DrawString(str, TextFont, Brushes.White, new PointF(0, line*TextFont.Height));
		//}

		//static void DrawString(Graphics gfx, string str, int line, float offset)
		//{
		//    gfx.DrawString(str, TextFont, Brushes.White, new PointF(offset, line*TextFont.Height));
		//}

		//static void DrawKeyboard(Graphics gfx, KeyboardState keyboard, int line)
		//{
		//    const string str="Keys pressed:";
		//    float space=gfx.MeasureString(" ", TextFont).Width;
		//    float offset=gfx.MeasureString(str, TextFont).Width+space;
		//    DrawString(gfx, str, line);
		//    for(int i=0; i<(int)Key.LastKey; i++)
		//    {
		//        Key k=(Key)i;
		//        if(keyboard[k])
		//        {
		//            string key=k.ToString();
		//            DrawString(gfx, key, line, offset);
		//            offset+=gfx.MeasureString(key, TextFont).Width+space;
		//        }
		//    }
		//}

		//static void DrawMouse(Graphics gfx, MouseState mouse, int line)
		//{
		//    const string str="Buttons pressed:";
		//    float space=gfx.MeasureString(" ", TextFont).Width;
		//    float offset=gfx.MeasureString(str, TextFont).Width+space;
		//    DrawString(gfx, str, line);
		//    for(int i=0; i<(int)MouseButton.LastButton; i++)
		//    {
		//        MouseButton b=(MouseButton)i;
		//        if(mouse[b])
		//        {
		//            string button=b.ToString();
		//            DrawString(gfx, button, line, offset);
		//            offset+=gfx.MeasureString(button, TextFont).Width+space;
		//        }
		//    }
		//}

		//protected override void OnUpdateFrame(FrameEventArgs e)
		//{
		//    mouse=OpenTK.Input.Mouse.GetState();
		//    if(mouse!=mouse_old)
		//        refresh_text=true;
		//    mouse_old=mouse;
		//    keyboard=OpenTK.Input.Keyboard.GetState();
		//    if(keyboard!=keyboard_old)
		//        refresh_text=true;
		//    keyboard_old=keyboard;

		//    if(refresh_text)
		//    {
		//        refresh_text=false;

		//        using(Graphics gfx=Graphics.FromImage(TextBitmap))
		//        {
		//            int line=0;

		//            gfx.Clear(Color.Black);
		//            gfx.TextRenderingHint=System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

		//            DrawString(gfx, String.Format("[1 - 4]: change WindowState (current: {0}).", this.WindowState), line++);
		//            DrawString(gfx, String.Format("[5 - 7]: change WindowBorder (current: {0}).", this.WindowBorder), line++);
		//            DrawString(gfx, String.Format("Focused: {0}.", this.Focused), line++);
		//            DrawString(gfx, String.Format("Mouse {0} window.", mouse_in_window?"inside":"outside of"), line++);
		//            DrawString(gfx, String.Format("Mouse visible: {0}", CursorVisible), line++);
		//            DrawString(gfx, String.Format("Mouse position (absolute): {0}", new Vector3(Mouse.X, Mouse.Y, Mouse.Wheel)), line++);
		//            DrawString(gfx, String.Format("Mouse position (relative): {0}", new Vector3(mouse.X, mouse.Y, mouse.WheelPrecise)), line++);
		//            DrawString(gfx, String.Format("Window.Bounds: {0}", Bounds), line++);
		//            DrawString(gfx, String.Format("Window.Location: {0}, Size: {1}", Location, Size), line++);
		//            DrawString(gfx, String.Format("Window.{{X={0}, Y={1}, Width={2}, Height={3}}}", X, Y, Width, Height), line++);
		//            DrawString(gfx, String.Format("Window.ClientRectangle: {0}", ClientRectangle), line++);
		//            DrawKeyboard(gfx, keyboard, line++);
		//            DrawMouse(gfx, mouse, line++);
		//        }
		//    }

		//    System.Drawing.Imaging.BitmapData data=TextBitmap.LockBits(
		//        new System.Drawing.Rectangle(0, 0, TextBitmap.Width, TextBitmap.Height),
		//        System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
		//    GL.TexSubImage2D(TextureTarget.Texture2D, 0, 0, 0, TextBitmap.Width, TextBitmap.Height, PixelFormat.Bgra,
		//        PixelType.UnsignedByte, data.Scan0);
		//    TextBitmap.UnlockBits(data);
		//}


		//protected override void OnLoad(EventArgs e)
		//{
		//    base.OnLoad(e);

		//    GL.ClearColor(Color.MidnightBlue);

		//    GL.Enable(EnableCap.Texture2D);
		//    GL.Enable(EnableCap.Blend);
		//    GL.BlendFunc(BlendingFactorSrc.One, BlendingFactorDest.OneMinusSrcColor);

		//    texture=GL.GenTexture();
		//    GL.BindTexture(TextureTarget.Texture2D, texture);
		//    GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, TextBitmap.Width, TextBitmap.Height,
		//        0, PixelFormat.Bgra, PixelType.UnsignedByte, IntPtr.Zero);
		//    GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)All.Nearest);
		//    GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)All.Nearest);
		//}

		//protected override void OnResize(EventArgs e)
		//{
		//    base.OnResize(e);
		//    viewport_changed=true;
		//}

		//protected override void OnRenderFrame(FrameEventArgs e)
		//{
		//    base.OnRenderFrame(e);

		//    if(viewport_changed)
		//    {
		//        viewport_changed=false;

		//        GL.Viewport(0, 0, Width, Height);

		//        Matrix4 ortho_projection=Matrix4.CreateOrthographicOffCenter(0, Width, Height, 0, -1, 1);
		//        GL.MatrixMode(MatrixMode.Projection);
		//        GL.LoadMatrix(ref ortho_projection);
		//    }

		//    GL.Clear(ClearBufferMask.ColorBufferBit);

		//    GL.Begin(BeginMode.Quads);

		//    GL.TexCoord2(0, 0); GL.Vertex2(0, 0);
		//    GL.TexCoord2(1, 0); GL.Vertex2(TextBitmap.Width, 0);
		//    GL.TexCoord2(1, 1); GL.Vertex2(TextBitmap.Width, TextBitmap.Height);
		//    GL.TexCoord2(0, 1); GL.Vertex2(0, TextBitmap.Height);

		//    GL.End();

		//    SwapBuffers();
		//}

		//public static void Main()
		//{
		//    using(Program ex=new Program())
		//    {
		//        //Utilities.SetWindowTitle(ex);
		//        ex.Run(30.0);
		//    }
		//}

		  private Gwen.Input.OpenTK input;
        private Gwen.Renderer.OpenTK renderer;
        private Gwen.Skin.Base skin;
        private Gwen.Control.Canvas canvas;

        const int fps_frames = 50;
        private readonly List<long> ftime;
        private readonly Stopwatch stopwatch;
        private long lastTime;
        private bool altDown = false;

		public Program()
            : base(1024, 768)
        {
            Keyboard.KeyDown += Keyboard_KeyDown;
            Keyboard.KeyUp += Keyboard_KeyUp;

            Mouse.ButtonDown += Mouse_ButtonDown;
            Mouse.ButtonUp += Mouse_ButtonUp;
            Mouse.Move += Mouse_Move;
            Mouse.WheelChanged += Mouse_Wheel;

            ftime = new List<long>(fps_frames);
            stopwatch = new Stopwatch();
        }

        public override void Dispose()
        {
            canvas.Dispose();
            skin.Dispose();
            renderer.Dispose();
            base.Dispose();
        }

        /// <summary>
        /// Occurs when a key is pressed.
        /// </summary>
        /// <param name="sender">The KeyboardDevice which generated this event.</param>
        /// <param name="e">The key that was pressed.</param>
        void Keyboard_KeyDown(object sender, KeyboardKeyEventArgs e)
        {
            if (e.Key == global::OpenTK.Input.Key.Escape)
                Exit();
            else if (e.Key == global::OpenTK.Input.Key.AltLeft)
                altDown = true;
            else if (altDown && e.Key == global::OpenTK.Input.Key.Enter)
                if (WindowState == WindowState.Fullscreen)
                    WindowState = WindowState.Normal;
                else
                    WindowState = WindowState.Fullscreen;

            input.ProcessKeyDown(e);
        }

        void Keyboard_KeyUp(object sender, KeyboardKeyEventArgs e)
        {
            altDown = false;
            input.ProcessKeyUp(e);
        }

        void Mouse_ButtonDown(object sender, MouseButtonEventArgs args)
        {
            input.ProcessMouseMessage(args);
        }

        void Mouse_ButtonUp(object sender, MouseButtonEventArgs args)
        {
            input.ProcessMouseMessage(args);
        }

        void Mouse_Move(object sender, MouseMoveEventArgs args)
        {
            input.ProcessMouseMessage(args);
        }

        void Mouse_Wheel(object sender, MouseWheelEventArgs args)
        {
            input.ProcessMouseMessage(args);
        }

        /// <summary>
        /// Setup OpenGL and load resources here.
        /// </summary>
        /// <param name="e">Not used.</param>
        protected override void OnLoad(EventArgs e)
        {
            GL.ClearColor(Color.MidnightBlue);

            renderer = new Gwen.Renderer.OpenTK();
            skin = new Gwen.Skin.TexturedBase(renderer, "DefaultSkin.png");
            //skin = new Gwen.Skin.Simple(renderer);
            //skin.DefaultFont = new Font(renderer, "Courier", 10);
            canvas = new Canvas(skin);

            input = new Gwen.Input.OpenTK();
            input.Initialize(canvas);

            canvas.SetSize(Width, Height);
            canvas.ShouldDrawBackground = true;
            canvas.BackgroundColor = Color.FromArgb(255, 150, 170, 170);
            //canvas.KeyboardInputEnabled = true;

            stopwatch.Restart();
            lastTime = 0;

			//Debug //Ssk
			//Label label=new Label(canvas);
			//label.Text="Standard label (not autosized)";
			//label.SetBounds(10, 10, 100, 10);

			Label label=new Label(canvas);
			label.AutoSizeToContents=true;
			label.Text="Chinese: \u4E45\u6709\u5F52\u5929\u613F \u7EC8\u8FC7\u9B3C\u95E8\u5173";
			label.SetPosition(10, 30);
        }

        /// <summary>
        /// Respond to resize events here.
        /// </summary>
        /// <param name="e">Contains information on the new GameWindow size.</param>
        /// <remarks>There is no need to call the base implementation.</remarks>
        protected override void OnResize(EventArgs e)
        {
            GL.Viewport(0, 0, Width, Height);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadIdentity();
            GL.Ortho(0, Width, Height, 0, -1, 1);

            canvas.SetSize(Width, Height);
        }

        /// <summary>
        /// Add your game logic here.
        /// </summary>
        /// <param name="e">Contains timing information.</param>
        /// <remarks>There is no need to call the base implementation.</remarks>
        protected override void OnUpdateFrame(FrameEventArgs e)
        {
            if (ftime.Count == fps_frames)
                ftime.RemoveAt(0);

            ftime.Add(stopwatch.ElapsedMilliseconds - lastTime);
            lastTime = stopwatch.ElapsedMilliseconds;

            if (stopwatch.ElapsedMilliseconds > 1000)
            {
                stopwatch.Restart();

                if (renderer.TextCacheSize > 1000) // each cached string is an allocated texture, flush the cache once in a while in your real project
                    renderer.FlushTextCache();
            }
        }

        /// <summary>
        /// Add your game rendering code here.
        /// </summary>
        /// <param name="e">Contains timing information.</param>
        /// <remarks>There is no need to call the base implementation.</remarks>
        protected override void OnRenderFrame(FrameEventArgs e)
        {
            GL.Clear(ClearBufferMask.DepthBufferBit | ClearBufferMask.ColorBufferBit);
            canvas.RenderCanvas();

            SwapBuffers();
        }

        /// <summary>
        /// Entry point of this example.
        /// </summary>
        [STAThread]
        public static void Main()
        {
			using(Program example=new Program())
            {
                example.Title = "invertika";
                example.VSync = VSyncMode.Off; // to measure performance
                example.Run(0.0, 0.0);
            }
        }
	}
}
